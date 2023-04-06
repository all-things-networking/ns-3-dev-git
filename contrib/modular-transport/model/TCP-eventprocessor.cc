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
    TCPContext* newContext = dynamic_cast<TCPContext*>(c); //This is only a pointer, not a copy right?
    std::vector<MTEvent*> newEvents;
    std::vector<Packet> packetTobeSend;
    for(; //m_Nxt
     (newContext->m_Nxt < newContext->m_Una + newContext->m_Wnd)&&(newContext->m_Nxt<128);
     newContext->m_Nxt += newContext->m_segmentsize){
        MTTCPHeader outgoingHeader = MTTCPHeader();
        outgoingHeader.seqnum = newContext->m_Iss + newContext->m_Nxt; //Confirmed: first sequence number of a segment
        //std::cout<<"set seqnum to"<<outgoingHeader.seqnum<<std::endl;
        Packet P = Packet(
            newContext->data+newContext->m_Nxt,
            newContext->m_segmentsize);
        P.AddHeader(outgoingHeader);
        packetTobeSend.emplace_back(P);
        std::cout<<"Send ifpossible:"<<newContext->m_Iss + newContext->m_Nxt<<" to "<<newContext->m_Iss + newContext->m_Nxt+newContext->m_segmentsize<<std::endl;
        newContext->RTOTimer->start();
        //TODO: packet id other than seqnum
        double now = Simulator::Now().GetSeconds();
        newContext->startTime[newContext->m_Iss + newContext->m_Nxt+newContext->m_segmentsize] = now;
        newContext->isResend[newContext->m_Iss + newContext->m_Nxt+newContext->m_segmentsize] = false;
        //TimeExpire * timeevent = TimeExpire(0, newContext->m_Nxt, ns3::Simulator::Now().GetSeconds()+2)
        //newEvents.push_back(timeevent);
     }
     //std::cout<<"SendIfPossible loop end"<<std::endl;
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

    //Calculates RTO:
    if (newContext->m_Una < event->acknum){
        //new ack (what if latter arrive first)
        newContext->m_Wnd += newContext->m_segmentsize;
        newContext->m_Una = event->acknum;
        newContext->RTOTimer->reset(newContext->RTO);
    }
    if (!newContext->isResend[event->acknum]){
        double now = Simulator::Now().GetSeconds();
        float R = now - newContext->startTime[event->acknum];
        if (newContext->SRTT == 0){//first time
            newContext->SRTT = R;
            newContext->RTTVAR = R/2;
            //max (G, 4*RTTVAR), G is Clock Granularity
            newContext->RTO = newContext->SRTT + 4 *newContext->RTTVAR;
        }
        else{
            float alpha=1.0/8.0;
            float beta=1.0/4.0;
            newContext->RTTVAR  = (1 - beta) * newContext->RTTVAR + beta * std::abs(newContext->SRTT - R);
            newContext->SRTT  =(1 - alpha) * newContext->SRTT + alpha * R;
            newContext->RTO = newContext->SRTT + 4 *newContext->RTTVAR;
        }
    // max (rto,1)
        std::cout<<"Set RTO to "<<newContext->RTO<<std::endl;
    }
    std::cout<<"m_Una increased to: "<<event->acknum<<std::endl;
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
    std::cout<<"Timer Expired being processed"<<std::endl;
    //Update windowsize
    if(newContext->m_Una<128){//TODO: should be min(len(data))
        newContext->m_Wnd = std::max(newContext->m_Wnd/2, (uint32_t)1);
        //Resend first segment (first segment only)
        MTTCPHeader outgoingHeader = MTTCPHeader();
        outgoingHeader.seqnum = newContext->m_Una;
        newContext->isResend[newContext->m_Una+newContext->m_segmentsize]=true;
        newContext->RTOTimer->reset();
        newContext->RTO = newContext->RTO * 2;
        if(newContext->RTO < 3){
            newContext->RTO = 3.0;
        }
        Packet P = Packet(
        newContext->data+newContext->m_Una, //this assumes data's start is at 0 seqnum
        newContext->m_segmentsize);
        std::cout<<"Send Timer:"<<newContext->m_Una<<" to "<<newContext->m_Una+newContext->m_segmentsize<<std::endl;
        P.AddHeader(outgoingHeader);
        packetTobeSend.emplace_back(P);
    }

    EventProcessorOutput *Output = new EventProcessorOutput;
    Output->newEvents=newEvents;
    Output->updatedContext=newContext;
    Output->packetToSend=packetTobeSend;
    return Output;
} // namespace ns3
}
