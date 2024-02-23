/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "Big_Class.h"
#include "mt-eventprocessor.h"
#include "QUIC-Dispatcher.h"
#include "QUIC-Scheduler.h"
#include "QUIC_State.h"
#include "QUIC-Context.h"
#include "QUIC-Dispatcher.h"
//#include "QUIC-Receiver.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("QUIC_TEL");

NS_OBJECT_ENSURE_REGISTERED(QUIC_TEL);

TypeId
QUIC_TEL::GetTypeId()
{
    static TypeId tid = TypeId("ns3::QUIC_TEL")
                            .SetParent<IpL4Protocol>()
                            .SetGroupName("QUIC_TEL")
                            .AddConstructor<QUIC_TEL>();
    return tid;
}
QUIC_TEL::QUIC_TEL(){
    this->table = QUICState(this);
    this->scheduler = QUICScheduler();
    this->dispatcher = QUICDispatcher();
    NS_LOG_FUNCTION(this);
}

QUIC_TEL::QUIC_TEL(QUICScheduler* scheduler, QUICDispatcher* dispatcher)
{
    this->table = QUICState(this);
    this->scheduler = scheduler;
    this->dispatcher = dispatcher;
    NS_LOG_FUNCTION(this);
}

QUIC_TEL::~QUIC_TEL()
{
    NS_LOG_FUNCTION(this);
}

void QUIC_TEL::QUIC_TEL(QUICEvent* e)
{
    this->scheduler->AddEvent(e);
}

void QUIC_TEL::WriteToTable(int flow_id, QUICContext* context) {
    this->table.Write(flow_id, context);
}
void QUIC_TEL::Start(const Ipv4Address& saddr, const Ipv4Address& daddr, QUICContext* StartContext) {
    Mainloop();
}
void QUIC_TEL::Mainloop(){
    // This is the main loop of the transport layer
       // that calls the different components of our model
       // to process events
    while (!this->scheduler->isEmpty()){
         QUICEvent* e = this->scheduler->GetNextEvent();
         // this MTEventProcessor should stay unchange
         std::vector<MTEventProcessor*> ep= this->dispatcher->dispatch(e);
         QUICContext* ctx = this->table.GetVal(e->flow_id);

         std::vector<QUICEvent*> newEvents;
         MTContext* context;
         std::vector<Packet> packetToSend;

         // intermediate output for the chain of processors
         IntermediateOutput* intermOutput;
         EventProcessorOutput* epout = new EventProcessorOutput{newEvents, ctx, packetToSend, intermOutput};

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
                //recreate Header for outgoing
                std::cout << "Sending Packets out" << std::endl;
                this->SendPacket(&packet, ctx->saddr, ctx->daddr);
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
QUIC_TEL::SetNode(Ptr<Node> node)
{
    NS_LOG_FUNCTION(this);
    m_node = node;
}


void QUIC_TEL::NotifyNewAggregate()
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

void QUIC_TEL::SendPacket(Ptr<Packet> packet,
                             const Ipv4Address& saddr,
                             const Ipv4Address& daddr) const
{
    NS_LOG_FUNCTION(this << packet << saddr << daddr);
    // TODO:Use NS_LOG_LOGIC to record information about the segment/packet being sent out.
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

//NOT YET implemented
enum IpL4Protocol::RxStatus
QUIC_TEL::Receive(Ptr<Packet> packet,
                          const Ipv4Header& incomingIpHeader,
                          Ptr<Ipv4Interface> incomingInterface)
                          //MTScheduler chosenScheduler)
{
    this->receiver->Receive(this, packet, incomingIpHeader, incomingInterface);
    return IpL4Protocol::RX_OK;
}

void
QUIC_TEL::SetDownTarget(IpL4Protocol::DownTargetCallback callback)
{
    m_downTarget = callback;
}

int
QUIC_TEL::GetProtocolNumber() const
{
    return PROT_NUMBER;
}

IpL4Protocol::DownTargetCallback
QUIC_TEL::GetDownTarget() const
{
    return m_downTarget;
}

/* ****** ICMP ******** */
void
QUIC_TEL::ReceiveIcmp(Ipv4Address icmpSource,
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
QUIC_TEL::ReceiveIcmp(Ipv6Address icmpSource,
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
QUIC_TEL::Receive(Ptr<Packet> packet,
                  const Ipv6Header& incomingIpHeader,
                  Ptr<Ipv6Interface> interface)
{
    NS_LOG_UNCOND("ModularTransport: IPv6 Receive not supported");
    return IpL4Protocol::RX_ENDPOINT_UNREACH;
}

void
QUIC_TEL::SetDownTarget6(IpL4Protocol::DownTargetCallback6 callback)
{
    m_downTarget6 = callback;
}

IpL4Protocol::DownTargetCallback6
QUIC_TEL::GetDownTarget6() const
{
    return m_downTarget6;
}

} // ns3 namespace
