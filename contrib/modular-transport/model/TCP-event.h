#ifndef TCP_EVENT_H
#define TCP_EVENT_H
#include "mt-event.h"
namespace ns3{
class MTHeader;
class MTEvent;
enum TCPEventType {
    SEND_DATA,
    ACK_DATA,
    TIMEOUT
};
class TCPEvent: public MTEvent{
    public:
    TCPEvent(){};
    ~TCPEvent(){};
    TCPEventType Type;
};
class SendEvent: public TCPEvent{
    public:
    SendEvent();
    ~SendEvent(){};
    SendEvent(long time, int flow_id);
};

class AckEvent: public TCPEvent{
    public:
    uint32_t acknum;
    AckEvent();
    ~AckEvent(){};
    AckEvent(int flow_id,  uint32_t acknum);
};
class Timeout: public TCPEvent{
    public:
    Timeout();
    Timeout(int flow_id);
    ~Timeout(){};
};

}
#endif
