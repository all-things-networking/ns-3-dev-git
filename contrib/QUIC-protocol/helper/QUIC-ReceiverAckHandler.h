#ifndef QUIC_RECEIVER_ACK_HANDLER_H
#define QUIC_RECEIVER_ACK_HANDLER_H
#include "ns3/mt-eventprocessor.h"
#include "ns3/QUIC-Context.h"
#include "QUIC-Frame.h"
#include <vector>
#include <utility>


namespace ns3
{

/**
 * \brief The processor handling the acknowledgement events.
 */
class QUICAckHandler: public MTEventProcessor
{
public:
    QUICAckHandler();
    EventProcessorOutput* Process(MTEvent* e, EventProcessorOutput* epOut);
    bool IsValidEvent(MTEvent* e);
    void updateContextReceivePackets(int currentPacketNumber, QUICContext* newContext);
};


} // namespace ns3

#endif