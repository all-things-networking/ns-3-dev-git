#ifndef QUIC_TEL_PACKETINFO
#define QUIC_TEL_PACKETINFO

#include <vector>

struct PacketInfo {
  int packet_id;
  int time_sent;
  bool in_flight;
  bool ack_eliciting;
  int size;
  std::vector<int> stream_id;
  std::vector<int> frame_size; 
  std::vector<int> start_point;
};

#endif