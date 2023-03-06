#ifndef MT_RECEIVER_H
#define MT_RECEIVER_H
#include "ns3/node.h"
#include "ns3/ip-l4-protocol.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv6-address.h"
#include "ns3/sequence-number.h"
namespace ns3{
class ModularTransport;
class MTReceiver{
    public:
    MTReceiver();
    virtual enum IpL4Protocol::RxStatus Receive(
                                            ModularTransport* mt,
                                            Ptr<Packet> p,
                                            const Ipv4Header& incomingIpHeader,
                                            Ptr<Ipv4Interface> incomingInterface) = 0;
};
}
#endif
