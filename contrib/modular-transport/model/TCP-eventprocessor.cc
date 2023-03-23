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
    std::cout<<"ok context"<<std::endl;
    TCPContext* newContext = dynamic_cast<TCPContext*>(c);
    std::vector<MTEvent*> newEvents;

    //New Packets
    //Add window
    std::cout<<"SendIfPossible loop start"<<std::endl;
    std::vector<Packet> packetTobeSend;
    if (newContext->m_Wnd<=64)
    for(;
     newContext->m_Nxt < newContext->m_Una + newContext->m_Wnd;
     newContext->m_Nxt+=newContext->m_segmentsize){
        MTTCPHeader outgoingHeader = MTTCPHeader();
        outgoingHeader.seqnum = newContext->m_Iss + newContext->m_Nxt; //Confirmed: first sequence number of a segment
        std::cout<<"set seqnum to"<<outgoingHeader.seqnum<<std::endl;
        Packet P = Packet(
            newContext->data+newContext->m_Nxt,
            newContext->m_segmentsize);
        P.AddHeader(outgoingHeader);
        packetTobeSend.emplace_back(P);
        //TimeExpire * timeevent = TimeExpire(0, newContext->m_Nxt, ns3::Simulator::Now().GetSeconds()+2)
        //newEvents.push_back(timeevent);
     }
     std::cout<<"SendIfPossible loop end"<<std::endl;
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
    std::vector<MTEvent*> newEvents;
    std::vector<Packet> packetTobeSend;

    newContext->m_Wnd += newContext->m_segmentsize;
    //need an array here
    newContext->m_Una = event->seq;
    //SendEvent(time, flow_id)
    MTEvent* newEvent = new SendEvent(0, event->flow_id);
    newEvents.push_back(newEvent);


    EventProcessorOutput *Output = new EventProcessorOutput;
    Output->newEvents=newEvents;
    Output->updatedContext=newContext;
    Output->packetToSend=packetTobeSend;
    return Output;
} // namespace ns3

TimedResendHandler::TimedResendHandler():
MTEventProcessor()
{}
bool
TimedResendHandler::IsValidEvent(MTEvent e)
{
    return true;
}

EventProcessorOutput* TimedResendHandler::Process(MTEvent* e, MTContext* c){
    TCPContext* newContext = dynamic_cast<TCPContext*>(c);
    TimerExpire* event = dynamic_cast<TimerExpire*>(e);
    std::vector<MTEvent*> newEvents;
    std::vector<Packet> packetTobeSend;

    if (newContext->m_Una <= newContext->m_Iss + event->seqnum){ //check if Sack for this packet have been received
        if (Simulator::Now().GetSeconds() > event.EndTime){
            MTTCPHeader outgoingHeader = MTTCPHeader();
            newContext->m_Wnd = std::max(newContext->m_Wnd/2, 1);
            if (event->seq < newContext->m_Wnd + newContext->m_Una){
                outgoingHeader.seqnum = newContext->m_Iss + event->seq; //Confirmed: first sequence number of a segment
                std::cout<<"set seqnum to"<<outgoingHeader.seqnum<<std::endl;
                Packet P = Packet(
                    newContext->data+event->seq,
                    newContext->m_segmentsize);
                P.AddHeader(outgoingHeader);
                packetTobeSend.emplace_back(P);
                //TimeExpire * timeevent = TimeExpire(0, newContext->m_Nxt, ns3::Simulator::Now().GetSeconds()+2)
                //newEvents.push_back(timeevent);
            }//hmm else ? set Next back?
         }else{
        //push it back
            newEvents.push_back(event);
         }
    }

    EventProcessorOutput *Output = new EventProcessorOutput;
    Output->newEvents=newEvents;
    Output->updatedContext=newContext;
    Output->packetToSend=packetTobeSend;
    return Output;
} // namespace ns3
}
