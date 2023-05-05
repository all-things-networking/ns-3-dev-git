#include "QUICReceiver-Event.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"

namespace ns3{

ReceivePacketEvent::ReceivePacketEvent(long time, int flow_id, Packet* rcv){
    this->flow_id = flow_id;
    this->time = time;
    this->receivered = rcv;
}
}