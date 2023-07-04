#ifndef QUICRECEIVER_RECEIVE_PROCESSOR_H
#define QUICRECEIVER_RECEIVE_PROCESSOR_H
#include "ns3/mt-receivelogic.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"
#include "ns3/mt-eventprocessor.h"
#include <map>
namespace ns3{
class MTReceiver;
class ModularTransport;
class Ipv4Interface;
class QUICReceiveProcessor: public MTReceiveLogic {

    public:
    uint32_t next_acknum; //TODO: inital sequence nummber
    std::map<uint32_t, uint32_t> packetbuffer;
    QUICReceiveProcessor();
    enum IpL4Protocol::RxStatus Receive(ModularTransport* mt,
                                    Ptr<Packet> p,
                                    const Ipv4Header& incomingIpHeader,
                                    Ptr<Ipv4Interface> incomingInterface);

};
}
#endif