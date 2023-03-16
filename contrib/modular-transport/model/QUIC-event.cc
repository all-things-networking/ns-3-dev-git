#include "QUIC-event.h"

namespace ns3{

ReceivePacketEvent::ReceivePacketEvent(long time, int flow_id, MTPacket* rcv){
    this->flow_id = flow_id;
    this->time = time;
    this->receivered = static_cast<QUICPacket*>(rcv);
}
}