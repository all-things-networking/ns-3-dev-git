#include "mt-event.h"
#include "mt-header.h"
#include "mt-state.h"
#include "QUIC-Event.h"

#include "ns3/ip-l4-protocol.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv6-address.h"
#include "ns3/sequence-number.h"

namespace ns3
{

class Node;
class MTState;
class QUICScheduler;
class QUICDispatcher;
class MTReceiver;
class QUICContext;

class QUIC_TEL : public IpL4Protocol
{
  public:
    /**
     * \brief Get the type ID.
     * \return the object TypeId
     */
    static TypeId GetTypeId();
    static const uint8_t PROT_NUMBER = 0xcc; //!< protocol number (0xcc)
    QUIC_TEL();
    QUIC_TEL(QUICScheduler*, QUICDispatcher*);
    ~QUIC_TEL() override;

    // Delete copy constructor and assignment operator to avoid misuse
    QUIC_TEL(const QUIC_TEL&) = delete;
    QUIC_TEL& operator=(const QUIC_TEL&) = delete;
    /**
     initiate a flow by adding its state/context to the state table.
     pick a constant for flow id. Context can include anything you need
     for processing TCP packets, e.g., initial sequence number,
     window size, beginning of the window, total number of bytes to send, etc.
    */
    void Start(const Ipv4Address& saddr, const Ipv4Address& daddr, QUICContext* StartContext);
    /**
    main of simulation
    */
    void Mainloop();

    /**
     * Set Dispatcher associated with this stack
     * \param node the node
     */
    void SetDispatcher(QUICDispatcher*);

    void AddEventToScheduler(QUICEvent* e);

    void WriteToTable(int flow_id, QUICContext* context);

    /**
     * Set Scheduler associated with this stack
     * \param node the node
     */
    void SetScheduler(QUICScheduler*);
    /**
     * Set Scheduler associated with this stack
     * \param node the node
     */
    //void SetReceiver(QUICReceiver*);

    /**
     * Set node associated with this stack
     * \param node the node
     */
    void SetNode(Ptr<Node> node);

    /**
     * \brief Send a packet
     *
     * \param pkt The packet to send
     * \param outgoing The packet header
     * \param saddr The source Ipv4Address
     * \param daddr The destination Ipv4Address
     */
    void SendPacket(Ptr<Packet> pkt, const Ipv4Address& saddr, const Ipv4Address& daddr) const;

    // From IpL4Protocol
    enum IpL4Protocol::RxStatus Receive(Ptr<Packet> p,
                                        const Ipv4Header& incomingIpHeader,
                                        Ptr<Ipv4Interface> incomingInterface) override;
    enum IpL4Protocol::RxStatus Receive(Ptr<Packet> p,
                                        const Ipv6Header& incomingIpHeader,
                                        Ptr<Ipv6Interface> incomingInterface) override;

    void ReceiveIcmp(Ipv4Address icmpSource,
                     uint8_t icmpTtl,
                     uint8_t icmpType,
                     uint8_t icmpCode,
                     uint32_t icmpInfo,
                     Ipv4Address payloadSource,
                     Ipv4Address payloadDestination,
                     const uint8_t payload[8]) override;

    void ReceiveIcmp(Ipv6Address icmpSource,
                     uint8_t icmpTtl,
                     uint8_t icmpType,
                     uint8_t icmpCode,
                     uint32_t icmpInfo,
                     Ipv6Address payloadSource,
                     Ipv6Address payloadDestination,
                     const uint8_t payload[8]) override;

    void SetDownTarget(IpL4Protocol::DownTargetCallback cb) override;
    void SetDownTarget6(IpL4Protocol::DownTargetCallback6 cb) override;
    int GetProtocolNumber() const override;
    IpL4Protocol::DownTargetCallback GetDownTarget() const override;
    IpL4Protocol::DownTargetCallback6 GetDownTarget6() const override;

  protected:
    void DoDispose() override;

    /**
     * \brief Setup callbacks when aggregated to a node
     *
     * This function will notify other components connected to the node that a
     * new stack member is now connected. This will be used to notify Layer 3
     * protocol of layer 4 protocol stack to connect them together.
     * The aggregation is completed by setting the node in the transport layer,
     * linking it to the ipv4 or ipv6 and setting up other relevant state.
     */
    void NotifyNewAggregate() override;

  private:
    // scheduler
    // dispatcher
    // context
    MTState table;
    QUICScheduler* scheduler;
    QUICDispatcher* dispatcher;
    MTReceiver* receiver;
    Ptr<Node> m_node;                                //!< the node this stack is associated with
    IpL4Protocol::DownTargetCallback m_downTarget;   //!< Callback to send packets over IPv4
    IpL4Protocol::DownTargetCallback6 m_downTarget6; //!< Callback to send packets over IPv6
};

} // namespace ns3