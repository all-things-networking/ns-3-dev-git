#ifndef TCP_EVENT_H
#define TCP_EVENT_H
#include "mt-event.h"
namespace ns3{
class MTHeader;
class MTEvent;
enum TCPEventType {
    SEND_DATA,
    ACK_DATA
};
class TCPEvent: public MTEvent{
    virtual TCPEvent()=0;
    virtual ~TCPEvent()=0;
    public:
    TCPEventType Type;
}
class SendEvent: public TCPEvent{
    public:
    TCPEventType Type;
    SendEvent();
    ~SendEvent(){};
    SendEvent(long time, int flow_id);
};

class AckEvent: public TCPEvent{
    public:
    TCPEventType Type;
    int seq;
    AckEvent();
    ~AckEvent(){};
    AckEvent(int flow_id, int seq);
};
}
#endif
