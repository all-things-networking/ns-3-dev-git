#ifndef QUIC_LOSS_DETECTION_H
#define QUIC_LOSS_DETECTION_H
#include "../../mt-eventprocessor.h"

namespace ns3
{
class MTEvent;
class MTEventQueue;
class MTHeader;

/**
 * \brief The processor handling the loss detection events.
 */
class QUICLossDetection: public MTEventProcessor
{
public:
    QUICLossDetection();
    EventProcessorOutput* Process(MTEvent e, MTContext* c);
    bool IsValidEvent(MTEvent e);

    void HandleRecieveACK(Packet* pkt);

    Packet* CreateProbePacket();
    Packet* CreateRetryPacket();

private:
    int PTO_Timer;
    int k_time_threshold;
};
} // namespace ns3
#endif
