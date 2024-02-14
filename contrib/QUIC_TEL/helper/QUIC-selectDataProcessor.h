#ifndef QUIC_SEND_PACKET_H
#define QUIC_SEND_PACKET_H

#include <ctime> // std::time_t
#include <map>
#include <deque>
#include <vector>
#include "QUIC-Frame.h"
#include "QUIC-StreamHandler.h"
#include "QUIC-Stream.h"
#include "QUIC-selectDataProcessor.h"
#include "QUIC-PacketBuffer.h"

#include "../model/QUIC-Context.h"
#include "../model/QUIC-Event.h"
#include "ns3/mt-event.h"
#include "ns3/mt-eventprocessor.h"
#include "ns3/mt-state.h"
#include "ns3/node.h"
namespace ns3
{

class QUICFrame;
class QUICPacketBuffer;
class Packet;

/**
 * \brief The class for a QUIC selectDataProcessor
 */
// QUIC selectDataProcessor
class selectDataProcessor : public MTEventProcessor
{
public:
    selectDataProcessor();
    ~selectDataProcessor();

    void Process(SendEvent* e, QUICContext ctx, std::vector<QUICEvent *> events, interm_out out, tx_module tx);
};

} // namespace ns3

#endif
