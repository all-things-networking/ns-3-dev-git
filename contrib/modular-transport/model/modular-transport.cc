/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "modular-transport.h"
#include "mt-eventprocessor.h"
#include "mt-dispatcher.h"
#include "TCP-scheduler.h"
#include "TCP-dispatcher.h"
#include "TCP-eventprocessor.h"
#include "TCP-context.h"
#include "mt-scheduler.h"
#include "mt-event.h"
#include "mt-state.h"
#include "mt-context.h"
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

ModularTransport::ModularTransport()
{
    this->table =  MTState(this);
    NS_LOG_FUNCTION(this);
}

ModularTransport::~ModularTransport()
{
    NS_LOG_FUNCTION(this);
}
void ModularTransport::Start(
                             const Ipv4Address& saddr,
                             const Ipv4Address& daddr){
    // initiate a flow by adding its state/context to the state table.
    // pick a constant for flow id. Context can include anything you need
    // for processing TCP packets, e.g., initial sequence number,
    // window size, beginning of the window, total number of bytes to send, etc.
    int flow_id = 1;
    //this->table =  MTState(this); move this line to constructor
    auto context = TcpContext(flow_id);//Change to MTContext
    context.saddr = saddr;
    context.daddr = daddr;
    uint8_t data [128];
    for(int i=0;i<128;i++){
        data[i]=i;
    }
    context.data = data;
    table.Write(flow_id, &context);
    auto scheduler = TCPscheduler();
    long time = 1;
       // Then, create a "send" event to send the first window of packets for this
       // flow. This event will be processed by "Send if Possible" event processor
     MTEvent e = scheduler.CreateSendEvent(flow_id, time);
     scheduler.AddEvent(e);
     Mainloop(&scheduler);
}
void ModularTransport::Mainloop(MTScheduler* scheduler){
    // This is the main loop of the transport layer
       // that calls the different components of our model
       // to process events
    TCPDispatcher dispatcher = TCPDispatcher();
    while (!scheduler->isEmpty()){
         MTEvent e = scheduler->GetNextEvent();
         MTEventProcessor* ep = dispatcher.dispatch(e);
         MTContext* ctx = this->table.GetVal(e.flow_id);
         EventProcessorOutput* result = ep->Process(e, ctx);
         for (auto newEvent : result->newEvents)
         {
                 scheduler->AddEvent(newEvent);
         }
         for (auto packet : result->packetToSend)
         {
                MTHeader outgoing;
                //recreate Header for outgoing
                this->SendPacket(&packet, outgoing, ctx->saddr, ctx->daddr);
         }


         //Use rult's mtcontext to update table's context at id
         //addall every thing in first vector of result into schedular
    }
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
    NS_LOG_UNCOND("inside received");
    MTHeader recievedHeader;
    packet->RemoveHeader(recievedHeader);
    uint8_t *buffer = new uint8_t[packet->GetSize()];
    int size = packet->CopyData(buffer, packet->GetSize());
    NS_LOG_UNCOND("Received: size"<<size<<" ");
    for(int i=0;i<packet->GetSize();i++){
        std::cout<<unsigned(buffer[i])<<std::endl;
    }
    //chosenScheduler.OpsAfterRecieved(recievedHeader);
    //chosenScheduler.GenerateEventOnReceive(recievedHeader);
    //recievedHeader.OpsAfterRecieved(); //THis one returns a event
    NS_LOG_FUNCTION(this << packet << incomingIpHeader << incomingInterface);

    NS_LOG_UNCOND("Received packet in ModularTransport");
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
