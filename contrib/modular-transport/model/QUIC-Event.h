#ifndef QUIC_EVENT_H
#define QUIC_EVENT_H
#include "mt-event.h"
namespace ns3{
class MTHeader;
class MTEvent;
class SendEvent: public MTEvent{
    public:
    SendEvent();
    SendEvent(long time, int flow_id);
};
}
#endif
