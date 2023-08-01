#ifndef QUIC_INTERMEDIATE_OUTPUT_H
#define QUIC_INTERMEDIATE_OUTPUT_H

#include <cstdint>
#include <vector>

#include "ns3/mt-intermediateoutput.h"
#include "QUIC-Frame.h"

namespace ns3
{
class QUICIntermediateOutput: public IntermediateOutput {
public:
    // intermediate output for PacketDemultiplexer: a mapping of frames to streams
    std::vector<std::pair<QUICFrameHeader, std::string>> PacketDemultiplexerOut;
    std::vector<QUICFrame*> QUICFrameBuffer;
    bool ackEliciting;
    int currentPacketNumber;

    QUICIntermediateOutput(){};
};
}
#endif