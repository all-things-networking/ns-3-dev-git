#ifndef MT_QUIC_INTERMEDIATE_OUTPUT_H
#define MT_QUIC_INTERMEDIATE_OUTPUT_H

#include <cstdint>
#include <vector>

#include "mt-intermediate_output.h"
#include "QUICFrame.h"

namespace ns3
{
class QUICIntermediateOutput: public IntermediateOutput {
public:
    std::vector<std::pair<uint64_t, QUICFrame>> PacketDemultiplexerOut;

    QUICIntermediateOutput(){};
};
}
#endif