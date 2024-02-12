#ifndef QUIC_EVENT_H
#define QUIC_EVENT_H
#include "ns3/mt-event.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"
namespace ns3{

//////////////////////////// Sender ////////////////////////////
const int NO_STREAM_ID = -1;

// this will choose the event processor (from dispatcher)
enum EventType {
    SEND,
    ACK,
    TIMER,
    ADD_DATA,
    CONGESTION,
    EMPTY //THERE is no EMPTY in TEL, add this here temporarily
};

// This is the base event for QUIC. It uses an EventType for further filtering in the dispatcher
class QUICEvent: public MTEvent{
    public:
    QUICEvent();
    QUICEvent(long time, int flow_id, EventType type = EventType::EMPTY);
    EventType type = EventType::EMPTY;
};



class Frame {
    public:
    int data_length;
    int stream_id;
    int offset;
    std::string data; // Try to correspond this field with "stream data" in the TEL DSL
    Frame();
    Frame(int data_length, int stream_id, int offset, std::string data);
};

// Reponse event type from the receiver

class MTHeader;
class MTEvent;
class SendEvent: public QUICEvent{
    public:
    int flow_id;
    SendEvent();
    SendEvent(int flow_id);
};

class AckEvent: public QUICEvent{
    public:
    int flow_id;
    std::vector<int> pkt_nums; //list<int> pkt_nums;
    int largest;
    int ECT0_Count;
    int ECT1_Count;
    int ECN_CE_Count;
    AckEvent();
    AckEvent(int flow_id);
};

class AddDataEvent : public QUICEvent {
    public:
    bool server_side;
    bool direction;
    std::string data;//stream data;
    AddDataEvent();
    AddDataEvent();
};

class StreamEvent : public QUICEvent { // trying to match this up with the struct QuicStream in the TEL DSL, not sure if I am doing
    // NOT sure is this is written correctly
    public:
    std::string data; // stream data;
    int id;
    int frame_size_limit;
    int last_sent;
    std::vector<bool> acked_frames; // list<bool> acked_frames;
    StreamEvent();

    StreamEvent(std::string data, int id = NO_STREAM_ID, int frame_size_limit);
};

// This one would be modified after finishing all the implementation and before testing
class SenderEventCreator
{
    public:
    // These events are used mainly for testing right now - it creates the specific event based
    // on input
    MTEvent* CreateSendEvent(int, long);
    MTEvent* CreateSendPacketEvent(int flow_id, long time);
    MTEvent* CreateAddDataEvent(int flow_id, long time, std::string text, int stream);
    MTEvent* CreateACKPacketEvent(int flow_id, long time, int packetNum);
};
////////////////////////////////////////////////////////////////

//////////////////////////// Receiver //////////////////////////
/**
 * NOT SURE IF IT is useful for our translation: Event for application layer updates
 */
class AppUpdateEvent : public QUICEvent
{
    public:
    AppUpdateEvent();
    AppUpdateEvent(long time, int flow_id);
};

/**
 * TIMER in TEL DSL is currently empty
 */
class TimerEvent : public QUICEvent
{
    public:
    TimerEvent();
    //TimerEvent(long time, int flow_id);
};

/**
 * Congestion in TEL DSL
 */
class CongestionEvent : public QUICEvent
{
    public:
    int time_sent;
    CongestionPacketEvent();
    CongestionPacketEvent(int time_sent);
};

// This one would be modified after finishing all the implementation and before testing
class ReceiverEventCreator
{
    public:
    // These events are used mainly for testing right now - it creates the specific event based
    // on input
    MTEvent* CreateReceiveEvent(int, long, Packet* packet);
    Packet* CreateFakePacket(std::vector<std::string>& data,
                                int packetNumber,
                                bool setFinBit,
                                int streamOffset,
                                int streamContentOffset = 0);
};

/////////////////////////////////////////////////////////////
}
#endif