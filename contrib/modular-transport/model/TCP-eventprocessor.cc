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
SendIfPossible::SendIfPossible():
MTEventProcessor()
{}
bool
SendIfPossible::IsValidEvent(MTEvent e)
{
    return true;
}

EventProcessorOutput* SendIfPossible::Process(MTEvent* e, MTContext* c){
    TCPContext* ctx = dynamic_cast<TCPContext*>(c); //This is only a pointer, not a copy right?
    std::vector<MTEvent*> newEvents;
    std::vector<Packet> packetTobeSend;
    for(; //m_Nxt
     (ctx->m_Nxt < ctx->m_Una + ctx->m_Wnd)&&(ctx->m_Nxt<ctx->flow_size);
     ctx->m_Nxt += ctx->m_segmentsize){
        MTTCPHeader outgoingHeader = MTTCPHeader();
        outgoingHeader.seqnum = ctx->m_Iss + ctx->m_Nxt; //Confirmed: first sequence number of a segment
        //std::cout<<"set seqnum to"<<outgoingHeader.seqnum<<std::endl;
        Packet P = Packet(
            ctx->data+ctx->m_Nxt,
            ctx->m_segmentsize);
        P.AddHeader(outgoingHeader);
        packetTobeSend.emplace_back(P);
        std::cout<<"Send ifpossible:"<<ctx->m_Iss + ctx->m_Nxt<<" to "<<ctx->m_Iss + ctx->m_Nxt+ctx->m_segmentsize<<std::endl;
        ctx->RTOTimer->start();
        double now = Simulator::Now().GetSeconds();
        ctx->startTime[ctx->m_Iss + ctx->m_Nxt+ctx->m_segmentsize] = now;
        ctx->isResend[ctx->m_Iss + ctx->m_Nxt+ctx->m_segmentsize] = false;
        //TimeExpire * timeevent = TimeExpire(0, ctx->m_Nxt, ns3::Simulator::Now().GetSeconds()+2)
        //newEvents.push_back(timeevent);
     }
     //std::cout<<"SendIfPossible loop end"<<std::endl;
    //Create header here

    //Output
    EventProcessorOutput *Output = new EventProcessorOutput;
    Output->newEvents=newEvents;
    Output->updatedContext=ctx;
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
    TCPContext* ctx = dynamic_cast<TCPContext*>(c);
    AckEvent* event = dynamic_cast<AckEvent*>(e);
    std::vector<MTEvent*> newEvents;
    std::vector<Packet> packetTobeSend;

    //Calculates RTO:

    //Update if I got 2 consecutive new acks without a timeout, update RTO based on second packet
    if (!ctx->timeouthappend && ctx->m_Una < event->acknum){
        double now = Simulator::Now().GetSeconds();
        float R = now - ctx->startTime[event->acknum];
        if (ctx->SRTT == 0){//first time
            ctx->SRTT = R;
            ctx->RTTVAR = R/2;
            //max (G, 4*RTTVAR), G is Clock Granularity
            ctx->RTO = ctx->SRTT + 4 *ctx->RTTVAR;
        }
        else{
            float alpha=1.0/8.0;
            float beta=1.0/4.0;
            ctx->RTTVAR  = (1 - beta) * ctx->RTTVAR + beta * std::abs(ctx->SRTT - R);
            ctx->SRTT  =(1 - alpha) * ctx->SRTT + alpha * R;
            ctx->RTO = ctx->SRTT + 4 *ctx->RTTVAR;
        }
    // max (rto,1)
        std::cout<<"Set RTO to "<<ctx->RTO<<std::endl;
    }
    if (ctx->m_Una < event->acknum){
            ctx->m_Wnd += ctx->m_segmentsize;
            ctx->m_Una = event->acknum;
            ctx->RTOTimer->reset(ctx->RTO);
            ctx->timeouthappend = false;
    }
    std::cout<<"m_Una increased to: "<<event->acknum<<std::endl;
    MTEvent* newEvent = new SendEvent(0, event->flow_id);
    newEvents.push_back(newEvent);



    EventProcessorOutput *Output = new EventProcessorOutput;
    Output->newEvents=newEvents;
    Output->updatedContext=ctx;
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
    TCPContext* ctx = dynamic_cast<TCPContext*>(c);
    TimerExpire* event = dynamic_cast<TimerExpire*>(e);
    std::vector<MTEvent*> newEvents;
    std::vector<Packet> packetTobeSend;
    std::cout<<"Timer Expired being processed"<<std::endl;
    //Update windowsize
    ctx->timeouthappend = true;
    if(ctx->m_Una<ctx->flow_size){//TODO: should be min(len(data))
        ctx->m_Wnd = std::max(ctx->m_Wnd/2, (uint32_t)1);
        //Resend first segment (first segment only)
        MTTCPHeader outgoingHeader = MTTCPHeader();
        outgoingHeader.seqnum = ctx->m_Una;
        ctx->isResend[ctx->m_Una+ctx->m_segmentsize]=true;
        ctx->RTOTimer->reset();
        ctx->RTO = ctx->RTO * 2;
        if(ctx->RTO < 3){
            ctx->RTO = 3.0;
        }
        Packet P = Packet(
        ctx->data + ctx->m_Una, //this assumes data's start is at 0 seqnum
        ctx->m_segmentsize);
        std::cout<<"Send Timer:"<<ctx->m_Una<<" to "<<ctx->m_Una+ctx->m_segmentsize<<std::endl;
        P.AddHeader(outgoingHeader);
        packetTobeSend.emplace_back(P);
    }

    EventProcessorOutput *Output = new EventProcessorOutput;
    Output->newEvents=newEvents;
    Output->updatedContext=ctx;
    Output->packetToSend=packetTobeSend;
    return Output;
} // namespace ns3
}
