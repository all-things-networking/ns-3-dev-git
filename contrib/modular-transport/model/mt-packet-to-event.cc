#include "mt-packet-to-event.h"
#include "mt-event.h"

#include <ns3/Tcp-header.h>

#include <vector>


namespace ns3
{

MtPacketToEvent::MtPacketToEvent()
{
}

std::vector<MtEvent>
MtPacketToEvent::CreateEvents(const TcpHeader& incomingMtHeader)
{
    std::vector<MtEvent> result{};
    uint8_t packetType = incomingMtHeader.GetFlags()
    switch (packetType)
    {
      case MtHeader::ACK:
          uint32_t ackNum = incomingMtHeader.GetAckNumber().GetValue();
          // \todo Determine Mt connection id
          result.emplace_back(MtAckEvent(0, ackNum))
      default:
    }
    return result;
}

} // namespace ns3
