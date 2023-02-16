#include "mt-eventprocessor.h"
#include "mt-state.h"
#include "mt-event.h"
#include <vector>
#include <utility>   // std::pair
#include <algorithm> // std::min, std::max
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"
namespace ns3
{

class Packet;
class ModularTransport;
MTEventProcessor::MTEventProcessor(){

}
//Send if possible, mine
SendIfPossible::SendIfPossible():
MTEventProcessor()
{}
bool
SendIfPossible::IsValidEvent(MTEvent e)
{
    return true;
}

EventProcessorOutput* SendIfPossible::Process(MTEvent e, MTContext c){
    //I call mt->SendPack here
    MTContext newContext = c;
    std::vector<MTEvent> newEvents;
    Packet P = Packet();
    std::vector<Packet> packetTobeSend;
    packetTobeSend.emplace_back(P);
    EventProcessorOutput* Output = &{newEvents,&newContext,packetTobeSend};
    return Output;
    //store packets to send as vector in class
    //call get packet to retrieve it later, and clear vector in class, use temp vector
}
} // namespace ns3
