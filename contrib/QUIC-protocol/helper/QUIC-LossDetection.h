#ifndef QUIC_LOSS_DETECTION_H
#define QUIC_LOSS_DETECTION_H
#include "ns3/mt-eventprocessor.h"
#include "../model/QUIC-Event.h"
#include "QUIC-Frame.h"
namespace ns3
{
class MTEvent;
class MTEventQueue;
class MTHeader;
class QUICContext;

/**
 * \brief The processor handling the loss detection events. This class is not currently used ... maybe can delete this
 */
class QUICLossDetection: public MTEventProcessor
{
public:
    QUICLossDetection();
    EventProcessorOutput* Process(MTEvent* e, EventProcessorOutput* epOut);
    bool IsValidEvent(MTEvent* e);

    EventProcessorOutput* HandleReceiveACK(ResponseEvent* e, QUICContext* c);

    Packet* CreateProbePacket();
    Packet* CreateRetryPacket();

private:
    int PTO_Timer;
    int k_time_threshold;
};
} // namespace ns3
#endif
