#ifndef TCP_EVENT_H
#define TCP_EVENT_H
#include "mt-event.h"
namespace ns3{
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
}
#endif
