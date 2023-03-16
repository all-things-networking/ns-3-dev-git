#ifndef MT_QUIC_EVENT_H
#define MT_QUIC_EVENT_H
#include "mt-event.h"
#include "QUICPacket.h"

namespace ns3{
class AppUpdateEvent: public MTEvent{
    public:
    AppUpdateEvent();
    AppUpdateEvent(long time, int flow_id);
};

class ACKTimerEvent: public MTEvent{
    public:
    ACKTimerEvent();
    ACKTimerEvent(long time, int flow_id);
};

class ReceivePacketEvent: public MTEvent{
    public:
    QUICPacket* receivered;
    ReceivePacketEvent();
    ReceivePacketEvent(long time, int flow_id, MTPacket* rcv);
};
}
#endif