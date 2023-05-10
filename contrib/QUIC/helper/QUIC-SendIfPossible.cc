#include "ns3/mt-eventprocessor.h"
#include "ns3/mt-state.h"
#include "ns3/mt-event.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"
#include "../model/QUIC-Context.h"

#include <vector>
#include <utility>   // std::pair
#include <algorithm> // std::min, std::max

#include "QUIC-SendIfPossible.h"
namespace ns3
{

// TODO: This file needs to be adapted for QUIC. 

class Packet;
class ModularTransport;

//Send if possible, mine
QUICSendIfPossible::QUICSendIfPossible():
MTEventProcessor()
{}
bool
QUICSendIfPossible::IsValidEvent(MTEvent * e)
{
    return true;
}

EventProcessorOutput* QUICSendIfPossible::Process(MTEvent* e, EventProcessorOutput* epOut)
{
    MTContext * c = epOut->context;

    //I call mt->SendPack here
    QUICContext* newContext = dynamic_cast<QUICContext*>(c);
    //A
    std::vector<MTEvent*> newEvents;

    //New Packets
    //Add window
    std::vector<Packet> packetTobeSend;
    Packet P = Packet(newContext->data, 4);
    packetTobeSend.emplace_back(P);

    //Output
    EventProcessorOutput *Output = new EventProcessorOutput;
    Output->newEvents=newEvents;
    Output->context=newContext;
    Output->packetToSend=packetTobeSend;
    return Output;
    //store packets to send as vector in class
    //call get packet to retrieve it later, and clear vector in class, use temp vector
}
} // namespace ns3
