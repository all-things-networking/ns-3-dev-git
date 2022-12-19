/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "modular-transport.h"

#include "mt-dispatcher.h"
#include "mt-event.h"
#include "mt-eventprocessor.h"
#include "mt-packet-to-event.h"
#include "mt-scheduler.h"
#include "mt-state.h"
#include "mt-timer.h"

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
    : m_scheduler{MtScheduler()},
      m_dispatcher{MtDispatcher()},
      m_state{MtState(this)},
      m_packetToEvent{MtPacketToEvent()}
{
    NS_LOG_FUNCTION(this);

    // Add processors
    this->m_dispatcher.AddProcessor(TcpAckHandler());
    this->m_dispatcher.AddProcessor(TcpCongControl());
}

void
ModularTransport::Core()
{
    // Get the next event to process.
    MtEvent nextEvent = this->m_scheduler.NextEvent();
    // Get the corresponding processor.
    MtEventProcessor processor = this->m_dispatcher.Dispatch(nextEvent);
    // Get the tcp id of the event
    int tcpId = 0; // TODO: Need to find a way to get this and initialize this.
    // Get the context of the tcp id.
    TcpContext context = this->m_state.GetVal(tcpId);
    // Process the event.
    std::pair<std::vector<MtEvent>, MtContext> result = processor.Process(nextEvent, context);
    // Update context and push generated new events.
    this->m_state.Write(result.second());
    for (auto newEvent : result.first())
    {
        this->m_scheduler.PushInEvent(newEvent);
    }
}

void
ModularTransport::TimeOutNotify(int tcpId)
{
    MtEvent timeoutEvent = TcpTimeoutEvent(tcpId);
    this->m_scheduler.PushInEvent(timeoutEvent);
}

ModularTransport::~ModularTransport()
{
    NS_LOG_FUNCTION(this);
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
{
    NS_LOG_FUNCTION(this << packet << incomingIpHeader << incomingInterface);
    NS_LOG_UNCOND("Received packet in ModularTransport");
    return IpL4Protocol::RX_OK;

    // MT specific
    // Get the tcp header of the incoming packet
    TcpHeader tcpHeader;
    this->PacketReceived(packet, tcpHeader, incomingIpHeader.GetSource(), incomingIpHeader.GetDestination());
    // Convert to packet receive events.
    std::vector<MtEvent> eventList = this->m_packetToEvent.CreateEvents(packet);
    for (auto event : eventList)
    {
        this->m_scheduler.PushInEvent(event);
    }
    // TODO: need a way to create TCP id and create corresponding context.
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
