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
    //Send any data between m_Nxt to the end of the window
    for(; //m_Nxt
     (ctx->m_Nxt < ctx->m_Una + ctx->m_Wnd)&&(ctx->m_Nxt<ctx->flow_size);
     ctx->m_Nxt += ctx->m_segmentsize){
        MTTCPHeader outgoingHeader = MTTCPHeader();
        outgoingHeader.seqnum = ctx->m_Iss + ctx->m_Nxt;
        //std::cout<<"set seqnum to"<<outgoingHeader.seqnum<<std::endl;
        Packet P = Packet(
            ctx->data+ctx->m_Nxt,
            ctx->m_segmentsize);
        P.AddHeader(outgoingHeader);
        packetTobeSend.emplace_back(P);
        std::cout<<"Send ifpossible:"<<ctx->m_Iss + ctx->m_Nxt<<" to "<<ctx->m_Iss + ctx->m_Nxt+ctx->m_segmentsize<<std::endl;
        //Restart the timer if it is stopped or not started yet
        ctx->RTOTimer->start();
        //Tracking RTT
        double now = Simulator::Now().GetSeconds();
        ctx->startTime[ctx->m_Iss + ctx->m_Nxt+ctx->m_segmentsize] = now;
        ctx->isResend[ctx->m_Iss + ctx->m_Nxt+ctx->m_segmentsize] = false;
     }
    //Output
    EventProcessorOutput *Output = new EventProcessorOutput;
    Output->newEvents=newEvents;
    Output->updatedContext=ctx;
    Output->packetToSend=packetTobeSend;
    return Output;
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

    //Calculates RTTVAR, SRTT and RTO:

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
        if (ctx->RTO<1){
            ctx->RTO=1;
        }
        std::cout<<"Set RTO to "<<ctx->RTO<<std::endl;
    }
    //Update the window here when a new ack for new segment arrives
    if (ctx->m_Una < event->acknum){
            //TODO: expand the window update function
            ctx->m_Wnd += ctx->m_segmentsize;
            ctx->m_Una = event->acknum;
            ctx->RTOTimer->reset(ctx->RTO);
            ctx->timeouthappend = false;
            //Now the window size is increased, check if you can send new data
            MTEvent* newEvent = new SendEvent(0, event->flow_id);
            newEvents.push_back(newEvent);
    }
    std::cout<<"m_Una increased to: "<<event->acknum<<std::endl;
    EventProcessorOutput *Output = new EventProcessorOutput;
    Output->newEvents=newEvents;
    Output->updatedContext=ctx;
    Output->packetToSend=packetTobeSend;
    return Output;
} // namespace ns3

TimeoutHandler::TimeoutHandler():
MTEventProcessor()
{}
bool
TimeoutHandler::IsValidEvent(MTEvent e)
{
    return true;
}

EventProcessorOutput* TimeoutHandler::Process(MTEvent* e, MTContext* c){
    TCPContext* ctx = dynamic_cast<TCPContext*>(c);
    Timeout* event = dynamic_cast<Timeout*>(e);
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
