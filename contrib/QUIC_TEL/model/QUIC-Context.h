#ifndef QUIC_CONTEXT_H
#define QUIC_CONTEXT_H

#include <ctime> // std::time_t
#include <map>
#include <unordered_map>
#include <vector>
#include <queue>
#include "ns3/ipv4-address.h"
#include "ns3/mt-state.h"

#include "../helper/QUIC-Stream.h"
#include "../helper/QUIC-Stream.h"
#include "../helper/QUIC-PacketBuffer.h"

namespace ns3
{

class ModularTransport;
class QUICStream;

// Used to packet tracking in case re-transmission is needed
enum class PacketState {
  SENT,
  ACKED,
  NOT_SENT,
  RECEIVED
};

////////////////////////////NEW CLASS PacketInfo from TEL DSL////////////////////////////////
class PacketInfo {
  int packet_id;
  int time_sent;
  bool in_flight;
  bool ack_eliciting;
  int size;
  std::vector<int> stream_id; //(change the type from list to std::vector)
  std::vector<int> frame_size; //(change the type from list to std::vector)
  std::vector<int> start_point; //(change the type from list to std::vector)
};

class QUICContext : public MTContext
{ 
public:

    QUICContext(int flow_id);
    ~QUICContext();
    //mathcing with the field of MyContext from TEL DSL

    std::vector<QUICStream*> streams; // streams for this connection (list<QuicStream> streams;)
    std::vector<PacketInfo*> sent_packets; // list<PacketInfo> sent_packets;
    int curr_idx;
    int size_limit;
    int MAX_STREAMS;
    int id_counter = 0;
    int server_stream_counter = 0;
    int client_stream_counter = 0;
    int largest_acked_time = 0;
    int packet_size_limit;
    // loss detection
    int kPacketThresh = 3;
    int kTimeThreshold;
    int time_of_last_ack_eliciting_packet = 0; // change to time_t type?
    int largest_acked_packet = -1; // Question: is inf ok?
    int kGranularity;
    // vars
    int pto_count = 0;
    int latest_rtt = 0;
    int smoothed_rtt;
    int rttvar;
    int min_rtt = 0;
    int first_rtt_sample = 0;
    int loss_time;

    bool update_rtt = false;
    // congestion
    int initialWindow = 14720;
    int minimumWindow;
    float lossReductionFactor = 0.5;
    int persistentCongestionThreshold = 3;

    int max_datagram_size;
    int bytes_in_flight = 0;
    int congestion_window = 0;
    int congestion_recovery_start_time = 0;
    int ssthresh = -1;
    int ecn_ce_counters = 0;
    //////////////////////////////////////////////////////////////////FIELDS of old QUIC in ns3////////////////////////////////////////////////////////////////
    int flow_id;
    uint32_t max_data; // maximum amount of data that can be sent on the connection
    uint32_t max_stream_data; // maximum amount of data that can be sent on a stream
    uint8_t* data;

    //////////////////////////// Sender ////////////////////////////
    
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
    int maxData = 10000;      // hard coded limit
    const int frameLimit = 5;
    // uint8_t* receiver_data;
    
    std::map<int, QUICStream*> receiverBuffer;
    int ackBase = 0; // sequence number of oldest acked packet
    std::vector<std::pair<int, int>> receivedPackets;
    int ackElicitingPacketCount = 0;

    // TODO: put hard-coded constant data here

    std::unordered_map<int, int> streamDataSize;
    int currentTotalDataSize;
    int receiverPacketNumber = 1;
    ///////////////////////////////////////////////////////////////

};
} // namespace ns3


#endif