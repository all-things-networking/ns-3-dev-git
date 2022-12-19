#ifndef MT_PACKET_TO_EVENT_H
#define MT_PACKET_TO_EVENT_H

#include <vector>

namespace ns3
{

class MtEvent;
class TcpHeader;

/**
 * \brief The class to convert incoming Mt packet to Events for processing.
 */
class MtPacketToEvent
{
public:
    MtPacketToEvent();

    /**
     * \brief Generate events from incoming packets
     * \param incomingIpHeader packet's header
     * \return vector of events
     */
    std::vector<MtEvent> CreateEvents(const TcpHeader& incomingIpHeader);
};


} // namespace ns3

#endif
