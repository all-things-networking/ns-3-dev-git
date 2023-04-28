/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include <iostream>
#include "modular-transport.h"
#include "mt-eventprocessor.h"
#include "mt-dispatcher.h"
#include "QUIC-dispatcher.h"
#include "QUIC-scheduler.h"
#include "mt-scheduler.h"
#include "mt-event.h"
#include "mt-state.h"
#include "mt-receivelogic.h"
#include "QUIC-context.h"
#include "QUIC-header.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("ModularTransport");

NS_OBJECT_ENSURE_REGISTERED(ModularTransport);

TypeId
ModularTransport::GetTypeId()
{
    static TypeId tid = TypeId("ns3::ModularTransport")
                            .SetParent<IpL4Protocol>()
                            .SetGroupName("ModularTransport")
                            .AddConstructor<ModularTransport>();
    return tid;
}

ModularTransport::ModularTransport(){
    this->table =  MTState(this);
    NS_LOG_FUNCTION(this);
}

ModularTransport::ModularTransport(MTScheduler* scheduler, MTDispatcher* dispatcher, MTReceiveLogic * receiver)
{
    this->table =  MTState(this);
    this->scheduler = scheduler;
    this->dispatcher = dispatcher;
    this->receiver = receiver;
    NS_LOG_FUNCTION(this);
}

ModularTransport::~ModularTransport()
{
    NS_LOG_FUNCTION(this);
}
void ModularTransport::Start(
                             const Ipv4Address& saddr,
                             const Ipv4Address& daddr, MTContext* StartContext){
    // initiate a flow by adding its state/context to the state table.
    // pick a constant for flow id. Context can include anything you need
    // for processing TCP packets, e.g., initial sequence number,
    // window size, beginning of the window, total number of bytes to send, etc.
    int flow_id = 1;
    table.Write(flow_id, *StartContext);

    long time = 1;
    // Then, create a "Receive" event. 
    // This event will be processed by "QUICPacketDemultiplexer", "QUICBufferManagement" and "QUICAckHandler" event processor
     Packet* pkg = new Packet();
     MTEvent* e = this->scheduler->CreateReceiveEvent(flow_id, time, pkg);
     this->scheduler->AddEvent(e);
     Mainloop(this->scheduler);
}
void ModularTransport::Mainloop(MTScheduler* scheduler){
    // This is the main loop of the transport layer
       // that calls the different components of our model
       // to process events
    QUICDispatcher dispatcher = QUICDispatcher();
    while (!scheduler->isEmpty()){
         MTEvent* e = scheduler->GetNextEvent();

         // get the chain of event processors
         std::vector<MTEventProcessor*> ep = dispatcher.dispatch(e);
         MTContext ctx = this->table.GetVal(e->flow_id);
         std::vector<MTEvent*> newEvents;
         MTContext* context;
         std::vector<Packet> packetToSend;

         // intermediate output for the chain of processors
         IntermediateOutput intermOutput;
         EventProcessorOutput* epout = new EventProcessorOutput{newEvents, &ctx, packetToSend, intermOutput};
         
         // run through all processors
         for (auto processor : ep) 
         {
            epout = processor->Process(e, epout);
         }

         for (auto newEvent : epout->newEvents)
          {
                 scheduler->AddEvent(newEvent);
          }

         for (auto packet : epout->packetToSend)
         {
                MTHeader outgoing;
                //recreate Header for outgoing
                this->SendPacket(&packet, outgoing, ctx.saddr, ctx.daddr);
         }


         //Use rult's mtcontext to update table's context at id
         //addall every thing in first vector of result into schedular
    }
    NS_LOG_UNCOND("Finished main loop");
}
void
ModularTransport::DoDispose()
{
    NS_LOG_FUNCTION(this);
    m_node = nullptr;
    m_downTarget.Nullify();
    m_downTarget6.Nullify();
    IpL4Protocol::DoDispose();
}

void
ModularTransport::SetNode(Ptr<Node> node)
{
    NS_LOG_FUNCTION(this);
    m_node = node;
}

void
ModularTransport::NotifyNewAggregate()
{
    NS_LOG_FUNCTION(this);
    Ptr<Node> node = this->GetObject<Node>();
    Ptr<Ipv4> ipv4 = this->GetObject<Ipv4>();

    if (!m_node)
    {
        if (node && ipv4)
        {
            this->SetNode(node);
        }
    }

    // We set the down target to the IPv4 send function.
    if (ipv4 && m_downTarget.IsNull())
    {
        ipv4->Insert(this);
        this->SetDownTarget(MakeCallback(&Ipv4::Send, ipv4));
    }
    IpL4Protocol::NotifyNewAggregate();
}

void
ModularTransport::SendPacket(Ptr<Packet> packet,
                             const MTHeader& outgoing,
                             const Ipv4Address& saddr,
                             const Ipv4Address& daddr) const
{
    NS_LOG_FUNCTION(this << packet << saddr << daddr);
    // TODO:Use NS_LOG_LOGIC to record information about the segment/packet being sent out.

    MTHeader outgoingHeader = outgoing;
    //outgoingHeader.OpsBeforeSend();//Need a global state variable
    packet->AddHeader(outgoingHeader);

    Ptr<Ipv4> ipv4 = m_node->GetObject<Ipv4>();
    if (ipv4)
    {
        Ipv4Header header;
        header.SetSource(saddr);
        header.SetDestination(daddr);
        header.SetProtocol(PROT_NUMBER);
        Socket::SocketErrno errno_;
        Ptr<Ipv4Route> route;
        if (ipv4->GetRoutingProtocol())
        {
            route = ipv4->GetRoutingProtocol()->RouteOutput(packet, header, nullptr, errno_);
        }
        else
        {
            NS_LOG_ERROR("No IPV4 Routing Protocol");
            route = nullptr;
        }
        m_downTarget(packet, saddr, daddr, PROT_NUMBER, route);
    }
    else
    {
        NS_FATAL_ERROR("Trying to use ModularTransport on a node without an Ipv4 interface");
    }
}

enum IpL4Protocol::RxStatus
ModularTransport::Receive(Ptr<Packet> packet,
                          const Ipv4Header& incomingIpHeader,
                          Ptr<Ipv4Interface> incomingInterface)
                          //MTScheduler chosenScheduler)
{
    this->receiver->Receive(this, packet, incomingIpHeader, incomingInterface);
    return IpL4Protocol::RX_OK;
}

void
ModularTransport::SetDownTarget(IpL4Protocol::DownTargetCallback callback)
{
    m_downTarget = callback;
}

int
ModularTransport::GetProtocolNumber() const
{
    return PROT_NUMBER;
}

IpL4Protocol::DownTargetCallback
ModularTransport::GetDownTarget() const
{
    return m_downTarget;
}

/* ****** ICMP ******** */
void
ModularTransport::ReceiveIcmp(Ipv4Address icmpSource,
                              uint8_t icmpTtl,
                              uint8_t icmpType,
                              uint8_t icmpCode,
                              uint32_t icmpInfo,
                              Ipv4Address payloadSource,
                              Ipv4Address payloadDestination,
                              const uint8_t payload[8])
{
    NS_LOG_UNCOND("ModularTransport: ICMP over IPv4 is not supported");
}

void
ModularTransport::ReceiveIcmp(Ipv6Address icmpSource,
                              uint8_t icmpTtl,
                              uint8_t icmpType,
                              uint8_t icmpCode,
                              uint32_t icmpInfo,
                              Ipv6Address payloadSource,
                              Ipv6Address payloadDestination,
                              const uint8_t payload[8])
{
    NS_LOG_UNCOND("ModularTransport: ICMP over IPv6 is not supported");
}

/* ********** IPv6-related functions ************ */
enum IpL4Protocol::RxStatus
ModularTransport::Receive(Ptr<Packet> packet,
                          const Ipv6Header& incomingIpHeader,
                          Ptr<Ipv6Interface> interface)
{
    NS_LOG_UNCOND("ModularTransport: IPv6 Receive not supported");
    return IpL4Protocol::RX_ENDPOINT_UNREACH;
}

void
ModularTransport::SetDownTarget6(IpL4Protocol::DownTargetCallback6 callback)
{
    m_downTarget6 = callback;
}

IpL4Protocol::DownTargetCallback6
ModularTransport::GetDownTarget6() const
{
    return m_downTarget6;
}

} // ns3 namespace
