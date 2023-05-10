#include "QUIC-Context.h"

namespace ns3{
QUICContext::QUICContext(int flow_id){
    this->data = NULL;
    this->PacketBuffer = new QUICPacketBuffer;
}

QUICContext::~QUICContext(){}
} // namespace ns3