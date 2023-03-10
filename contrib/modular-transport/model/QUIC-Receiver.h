#ifndef QUIC_RECEIVER_H
#define QUIC_RECEIVER_H
#include "mt-receiver.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"
namespace ns3{
class MTReceiver;
class ModularTransport;
class Ipv4Interface;
class QUICReceiver: public MTReceiver {

    public:
    QUICReceiver();
    enum IpL4Protocol::RxStatus Receive(ModularTransport* mt,
                                    Ptr<Packet> p,
                                    const Ipv4Header& incomingIpHeader,
                                    Ptr<Ipv4Interface> incomingInterface);

};
}
#endif
