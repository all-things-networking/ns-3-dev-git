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

namespace ns3
{

class ModularTransport;
class QUICStream;

////////////////////////////NEW CLASS PacketInfo from TEL DSL////////////////////////////////
class PacketInfo {
  public:
  int packet_id;
  time_t time_sent;
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

    QUICContext();
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
};
} // namespace ns3


#endif