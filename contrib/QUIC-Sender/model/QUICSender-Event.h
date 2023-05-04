#ifndef QUIC_EVENT_H
#define QUIC_EVENT_H
#include "ns3/mt-event.h"
#include "ns3/ipv4-l3-protocol.h"

namespace ns3{

class Packet;
const int NO_STREAM_ID = -1;

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
    StreamEventType streamEventType;
    int stream_id;
    StreamEvent();

    StreamEventData data;
    StreamEvent(int flow_id, StreamEventType streamEventType, StreamEventData data, int stream_id = NO_STREAM_ID);
};

class ResponseEvent: public MTEvent{
    public:
    ResponseEventType responseEventType;
    int stream_id; // Might not be needed in response event
    ResponseEventData data;

    ResponseEvent();
    ResponseEvent(int flow_id, ResponseEventType responseEventType, ResponseEventData data, int stream_id = NO_STREAM_ID);
};

}
#endif
