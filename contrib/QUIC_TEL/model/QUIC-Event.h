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
    std::string data; // Try to correspond this field with stream data in the TEL DSL
};

// Reponse event type from the receiver

class MTHeader;
class MTEvent;
class SendEvent: public QUICEvent{
    public:
    SendEvent();
    SendEvent(int flow_id);
};

class AckEvent: public QUICEvent{
    public:
    int seq;
    AckEvent();
    /*
    list<int> pkt_nums;
    int largest;
    int ECT0_Count;
    int ECT1_Count;
    int ECN_CE_Count;
    */
    AckEvent(int flow_id, int seq);
};

class StreamEvent: public QUICEvent{
    public:
    StreamEventType streamEventType; // This is used to identify the type of stream event
    int stream_id;
    StreamEvent();

    StreamEventData data;
    StreamEvent(int flow_id, StreamEventType streamEventType, StreamEventData data, int stream_id = NO_STREAM_ID);
};

class ResponseEvent: public QUICEvent{
    public:
    ResponseEventType responseEventType; // This is used to identify the type of response event
    int stream_id; // Might not be needed in response event
    ResponseEventData data;

    ResponseEvent();
    ResponseEvent(int flow_id, ResponseEventType responseEventType, ResponseEventData data, int stream_id = NO_STREAM_ID);
};

class SenderEventCreator {
    public:
    // These events are used mainly for testing right now - it creates the specific event based on input
    MTEvent* CreateSendEvent(int, long);
    MTEvent* CreateSendPacketEvent(int flow_id, long time);
    MTEvent* CreateAddDataEvent(int flow_id, long time, std::string text, int stream);
    MTEvent* CreateACKPacketEvent(int flow_id, long time, int packetNum);
};
////////////////////////////////////////////////////////////////

//////////////////////////// Receiver //////////////////////////
/**
 * Event for application layer updates
*/
class AppUpdateEvent: public QUICEvent{
    public:
    AppUpdateEvent();
    AppUpdateEvent(long time, int flow_id);
};

/**
 * Event for tracking ACK time
*/
class ACKTimerEvent: public QUICEvent{
    public:
    ACKTimerEvent();
    ACKTimerEvent(long time, int flow_id);
};

/**
 * Event for receiving a packet
*/
class ReceivePacketEvent: public QUICEvent{
    public:
    Packet* receivered;
    ReceivePacketEvent();
    ReceivePacketEvent(long time, int flow_id, Packet* rcv);
};

class ReceiverEventCreator {
    public:
    // These events are used mainly for testing right now - it creates the specific event based on input
    MTEvent* CreateReceiveEvent(int, long, Packet* packet);
    Packet* CreateFakePacket(std::vector<std::string>& data, int packetNumber, bool setFinBit, int streamOffset, int streamContentOffset = 0);
};


/////////////////////////////////////////////////////////////
}
#endif