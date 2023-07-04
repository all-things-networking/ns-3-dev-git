#ifndef QUIC_CONTEXT_H
#define QUIC_CONTEXT_H

#include <ctime> // std::time_t
#include <map>
#include <vector>
#include "ns3/ipv4-address.h"
#include "ns3/mt-state.h"

#include "../helper/QUIC-Stream.h"
#include "../helper/QUIC-Stream.h"
#include "../helper/QUIC-PacketBuffer.h"

namespace ns3
{

class ModularTransport;
class QUICStream;
enum class PacketState {
  SENT,
  ACKED,
  NOT_SENT,
  RECEIVED
};

// QUIC context
class QUICContext: public MTContext
{ 
public:

    QUICContext(int flow_id);
    ~QUICContext();

    int flow_id;
    uint32_t max_data; // maximum amount of data that can be sent on the connection
    uint32_t max_stream_data; // maximum amount of data that can be sent on a stream
    uint8_t* data;

    //////////////////////////// Sender ////////////////////////////
    std::map<int, QUICStream*> quic_streams; // streams for this connection
    int CurrentStream = 0; // Current stream used for multiplexing. This is the INDEX in quic_streams
    QUICPacketBuffer* PacketBuffer;

    // Keep track of sent packets for loss detection and error handling;
    std::deque<std::pair<Ptr<Packet>, PacketState>> sentPackets;
    int sendBase = 0; // Sequence number of OLDEST in-flight and unACKED packet
    int windowSize = 1; // [sendBase, sendBase + windowSize] is the range of packets in-flight (each may or may not be ACKED)
    int PTO_Timer;
    int k_packet_threshold = 1;
    ///////////////////////////////////////////////////////////////

    //////////////////////////// Receiver /////////////////////////
    // flow_id         (common)
    // max_data        (common)
    // max_stream_data (common)
    uint8_t* receiver_data;

    // quic_stream     (common)
    // PacketBuffer    (common)
    
    // std::deque<std::pair<Ptr<Packet>, PacketState>> receivedPackets;
    std::map<int, QUICStream*> receiverBuffer;
    int ackBase = 0; // sequence number of oldest acked packet
    // windowSize      (common)
    // PTO_Timer  only used in sender
    // k_packet_threshold?
    // add other things as needed
    ///////////////////////////////////////////////////////////////

};
} // namespace ns3


#endif