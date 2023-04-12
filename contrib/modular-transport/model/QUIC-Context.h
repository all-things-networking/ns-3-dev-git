#ifndef QUIC_CONTEXT_H
#define QUIC_CONTEXT_H

#include <ctime> // std::time_t
#include <map>
#include "ns3/ipv4-address.h"
#include "mt-context.h"
#include "./QUIC-sender/QUIC-Stream.h"
#include "./QUIC-sender/QUIC-PacketBuffer.h"

namespace ns3
{
    
class QUICStream;

enum class PacketState {
  SENT,
  ACKED,
  NOT_SENT
};

class MTContext;
/**
 * \brief The class for maintaing the state variables for ONE QUIC connection.
 */
// QUIC context
class QUICContext: public MTContext
{
public:
    QUICContext(int id);
    ~QUICContext();
    uint8_t* data;
    std::map<int, QUICStream*> quic_streams; // streams for this connection
    int CurrentStream = 0; // Current stream used for multiplexing. This is the INDEX in quic_streams
    QUICPacketBuffer* PacketBuffer;

    // Keep track of sent packets for loss detection and error handling;
    std::deque<std::pair<Ptr<Packet>, PacketState>> sentPackets;
};

} // namespace ns3

#endif
