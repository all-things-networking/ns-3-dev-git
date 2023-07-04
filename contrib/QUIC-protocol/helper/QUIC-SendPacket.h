#ifndef QUIC_SEND_PACKET_H
#define QUIC_SEND_PACKET_H

#include <ctime> // std::time_t
#include <map>
#include <deque>
#include "QUIC-Frame.h"
#include "QUIC-StreamHandler.h"
#include "QUIC-Stream.h"
#include "QUIC-SendPacket.h"

#include "../model/QUIC-Context.h"
#include "../model/QUIC-Event.h"
#include "ns3/mt-event.h"
#include "ns3/mt-eventprocessor.h"
#include "ns3/mt-state.h"
#include "ns3/node.h"
namespace ns3
{

class QUICFrame;
class Packet;

/**
 * \brief The class for a QUIC SendPacket
 */
// QUIC SendPacket
class QUICSendPacket
{
public:
    QUICSendPacket();
    ~QUICSendPacket();

    EventProcessorOutput* TrySendPacket(StreamEvent* e, QUICContext* c);
};

} // namespace ns3

#endif
