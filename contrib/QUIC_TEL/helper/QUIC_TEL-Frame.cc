#include "QUIC_TEL-Frame.h"
#include <vector>

namespace ns3
{
    Frame::Frame(int data_length, int stream_id, int offset) {
        this->data_length = data_length;
        this->stream_id = stream_id;
        this->offset = offset;
    };
} // namespace ns3