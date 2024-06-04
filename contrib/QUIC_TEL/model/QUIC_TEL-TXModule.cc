#include "QUIC_TEL-TXModule.h"
#include "ns3/core-module.h"

QUICTXModule::send(Packet *pkt) {
    // Hardcoded source and dest addr for now.
    Ptr<Node> src = *nodes.Begin();
    Ptr<Ipv4Interface> src_intf = src->GetObject<Ipv4L3Protocol>()->GetInterface(1);
    Ipv4Address saddr = src_intf->GetAddress(0).GetAddress();
    Ptr<Node> dst = *(nodes.End() - 1);
    Ipv4Address daddr = dst->GetObject<Ipv4L3Protocol>()->GetInterface(1)->GetAddress(0).GetAddress();
    this->quic->SendPacket(pkt, saddr, daddr);
}