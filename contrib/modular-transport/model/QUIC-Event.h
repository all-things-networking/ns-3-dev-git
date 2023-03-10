#ifndef QUIC_EVENT_H
#define QUIC_EVENT_H
#include "mt-event.h"
namespace ns3{

enum StreamEventType {
    SEND_DATA
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
    StreamEvent(int flow_id, StreamEventType streamEventType, int stream_id);
};

}
#endif
