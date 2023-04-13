#ifndef MT_QUIC_INTERMEDIATE_OUTPUT_H
#define MT_QUIC_INTERMEDIATE_OUTPUT_H

#include <cstdint>
#include <vector>

#include "mt-intermediate_output.h"
#include "QUIC-Frame.h"

namespace ns3
{
class QUICIntermediateOutput: public IntermediateOutput {
public:
    // intermediate output for PacketDemultiplexer: a mapping of frames to streams
    std::vector<std::pair<uint64_t, QUICFrame>> PacketDemultiplexerOut;

    QUICIntermediateOutput(){};
};
}
#endif