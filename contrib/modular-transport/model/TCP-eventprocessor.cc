#include "mt-eventprocessor.h"
#include "mt-state.h"
#include "mt-event.h"
#include "mt-header.h"
#include "TCP-header.h"
#include "TCP-event.h"
#include "TCP-eventprocessor.h"
#include "TCP-context.h"
#include <vector>
#include <utility>   // std::pair
#include <algorithm> // std::min, std::max
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"

//#TODO Ack processing
namespace ns3
{
class Packet;
class ModularTransport;
//Send if possible, mine
SendIfPossible::SendIfPossible():
MTEventProcessor()
{}
bool
SendIfPossible::IsValidEvent(MTEvent e)
{
    return true;
}

EventProcessorOutput* SendIfPossible::Process(MTEvent* e, MTContext* c){
    //I call mt->SendPack here
    TCPContext* newContext = dynamic_cast<TCPContext*>(c);
    //A
    std::vector<MTEvent> newEvents;

    //New Packets
    //Add window
    std::vector<Packet> packetTobeSend;
    if (newContext->m_Wnd<32)
    for(; newContext->m_Nxt < newContext->m_Una + newContext->m_Wnd; newContext->m_Nxt+=newContext->segmentsize){
        MTTCPHeader outgoingHeader = MTTCPHeader();
        outgoingHeader.seqnum = newContext->m_Iss + newContext->m_Nxt; //Confirmed: first sequence number of a segment
        Packet P = Packet(newContext->data+newContext->m_Nxt, newContext->segmentsize);
        P.AddHeader(outgoingHeader);
        packetTobeSend.emplace_back(P);
     }
    //Create header here

    //Output
    EventProcessorOutput *Output = new EventProcessorOutput;
    Output->newEvents=newEvents;
    Output->updatedContext=newContext;
    Output->packetToSend=packetTobeSend;
    return Output;
    //store packets to send as vector in class
    //call get packet to retrieve it later, and clear vector in class, use temp vector
}
AckHandler::AckHandler():
MTEventProcessor()
{}
bool
AckHandler::IsValidEvent(MTEvent e)
{
    return true;
}

EventProcessorOutput* AckHandler::Process(MTEvent* e, MTContext* c){
    TCPContext* newContext = dynamic_cast<TCPContext*>(c);
    AckEvent* event = dynamic_cast<AckEvent*>(e);
    std::vector<MTEvent> newEvents;
    std::vector<Packet> packetTobeSend;

    newContext->m_Wnd += newContext->segmentsize;
    newContext->m_Una = e->seq;
    //SendEvent(time, flow_id)
    MTEvent* newEvent = new SendEvent(0, event->flow_id);
    packetTobeSend.push_back(newEvent);

    EventProcessorOutput *Output = new EventProcessorOutput;
    Output->newEvents=newEvents;
    Output->updatedContext=newContext;
    Output->packetToSend=packetTobeSend;
    return Output;
} // namespace ns3
