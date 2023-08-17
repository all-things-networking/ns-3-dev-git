#ifndef QUIC_RECEIVER_PACKET_GENERATOR_H
#define QUIC_RECEIVER_PACKET_GENERATOR_H
#include "ns3/mt-eventprocessor.h"
#include "QUIC-Frame.h"
#include <vector>
#include <utility>
#include <unordered_set>
#include "../model/QUIC-Context.h"


namespace ns3
{

class QUICReceiverPacketGenerator: public MTEventProcessor
{
public:
    QUICReceiverPacketGenerator();
    EventProcessorOutput* Process(MTEvent* e, EventProcessorOutput* epOut);
    bool IsValidEvent(MTEvent* e);
};

} // namespace ns3

#endif