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
    //A
    std::vector<MTEvent> newEvents;

    //New Packets

    std::vector<Packet> packetTobeSend;
    Packet P = Packet(c->data, 4);
    packetTobeSend.emplace_back(P);

    //Output
    EventProcessorOutput *Output = new EventProcessorOutput;
    Output->newEvents=newEvents;
    Output->updatedContext=&newContext;
    Output->packetToSend=packetTobeSend;
    return Output;
    //store packets to send as vector in class
    //call get packet to retrieve it later, and clear vector in class, use temp vector
}
} // namespace ns3
