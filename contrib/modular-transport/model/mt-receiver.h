#ifndef MT_RECEIVER_H
#define MT_RECEIVER_H
#include "ns3/ip-l4-protocol.h"
#include "ns3/ipv4-interface.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"

namespace ns3{
class ModularTransport;
class Packet;
class IpL4Protocol;
class Ipv4Header;
class Ipv4Interface;

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
