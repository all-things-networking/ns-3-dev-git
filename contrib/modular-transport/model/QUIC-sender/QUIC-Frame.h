#ifndef QUIC_FRAME_H
#define QUIC_FRAME_H
#include <ctime> // std::time_t
#include <map>
#include "ns3/ipv4-l3-protocol.h"

namespace ns3
{

class Packet;

enum class FrameState {
  SENT,
  ACKED,
  NOT_SENT
};

/**
 * \brief The class for a QUIC Frame
 */
// QUIC Frame
class QUICFrame
{
public:
    QUICFrame(int size);
    QUICFrame();
    ~QUICFrame();
    
    int GetSize();

    int size;
    int seqnum; // 32 bits
    FrameState state;
    Ptr<Packet> data;


};

} // namespace ns3

#endif
