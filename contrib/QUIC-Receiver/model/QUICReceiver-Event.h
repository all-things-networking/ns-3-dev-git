#ifndef QUICRECEIVER_EVENT_H
#define QUICRECEIVER_EVENT_H
#include "ns3/mt-event.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"
namespace ns3{

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
}
#endif