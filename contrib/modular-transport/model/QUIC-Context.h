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
    QUICPacketBuffer* PacketBuffer;
};

} // namespace ns3

#endif
