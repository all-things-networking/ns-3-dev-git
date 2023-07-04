#ifndef QUIC_EVENT_H
#define QUIC_EVENT_H
#include "ns3/mt-event.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"
namespace ns3{

//////////////////////////// Sender ////////////////////////////
const int NO_STREAM_ID = -1;

// Stream level event type
enum StreamEventType {
    ADD_DATA,
    SEND_PACKET,
};

class StreamEventData {
    public:
    int packetNum;
    std::string text;
};

class ResponseEventData {
    public:
    int packetNum;
    std::string text;
};

// Reponse event type from the receiver
enum ResponseEventType {
    ACK_PACKET,
    NACK_PACKET
};

class MTHeader;
class MTEvent;
class SendEvent: public MTEvent{
    public:
    SendEvent();
    SendEvent(long time, int flow_id);
};

class AckEvent: public MTEvent{
    public:
    int seq;
    AckEvent();
    AckEvent(int flow_id, int seq);
};

class StreamEvent: public MTEvent{
    public:
    StreamEventType streamEventType; // This is used to identify the type of stream event
    int stream_id;
    StreamEvent();

    StreamEventData data;
    StreamEvent(int flow_id, StreamEventType streamEventType, StreamEventData data, int stream_id = NO_STREAM_ID);
};

class ResponseEvent: public MTEvent{
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
class AppUpdateEvent: public MTEvent{
    public:
    AppUpdateEvent();
    AppUpdateEvent(long time, int flow_id);
};

/**
 * Event for tracking ACK time
*/
class ACKTimerEvent: public MTEvent{
    public:
    ACKTimerEvent();
    ACKTimerEvent(long time, int flow_id);
};

/**
 * Event for receiving a packet
*/
class ReceivePacketEvent: public MTEvent{
    public:
    Packet* receivered;
    ReceivePacketEvent();
    ReceivePacketEvent(long time, int flow_id, Packet* rcv);
};

class ReceiverEventCreator {
    public:
    // These events are used mainly for testing right now - it creates the specific event based on input
    MTEvent* CreateReceiveEvent(int, long, Packet* packet);
    Packet* CreateFakePacket();
};

/////////////////////////////////////////////////////////////
}
#endif