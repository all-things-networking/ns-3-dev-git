#ifndef TCP_RECEIVER_H
#define TCP_RECEIVER_H
#include "mt-receivelogic.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"
#include <map>
namespace ns3{
class MTReceiver;
class ModularTransport;
class Ipv4Interface;
class TCPReceiveLogic: public MTReceiveLogic {

    public:
    uint32_t next_acknum; //TODO: inital sequence nummber
    std::map<uint32_t, uint32_t> buffer;
    TCPReceiveLogic();
    enum IpL4Protocol::RxStatus Receive(ModularTransport* mt,
                                    Ptr<Packet> p,
                                    const Ipv4Header& incomingIpHeader,
                                    Ptr<Ipv4Interface> incomingInterface);

};
}
#endif
