#ifndef QUIC_EVENT_H
#define QUIC_EVENT_H
#include "mt-event.h"
#include "ns3/ipv4-l3-protocol.h"

namespace ns3{

class Packet;
const int NO_STREAM_ID = -1;

enum StreamEventType {
    ADD_DATA,
    SEND_PACKET
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

    // Send Data 
    Ptr<Packet> data;
    StreamEvent(int flow_id, StreamEventType streamEventType, Ptr<Packet> data, int stream_id = NO_STREAM_ID);
};

}
#endif
