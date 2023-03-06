#ifndef TCP_RECEIVER_H
#define TCP_RECEIVER_H
#include "mt-receiver.h"
#include "ns3/node.h"
#include "ns3/ip-l4-protocol.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv6-address.h"
#include "ns3/sequence-number.h"
namespace ns3{
class MTReceiver;
class ModularTransport;
class Ipv4Interface;
class TCPReceiver: public MTReceiver {

    public:
    TCPReceiver();
    enum IpL4Protocol::RxStatus Receive(ModularTransport* mt,
                                    Ptr<Packet> p,
                                    const Ipv4Header& incomingIpHeader,
                                    Ptr<Ipv4Interface> incomingInterface);

};
}
#endif
