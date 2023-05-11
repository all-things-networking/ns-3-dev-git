#ifndef QUIC_APP_COMMUNICATION_H
#define QUIC_APP_COMMUNICATION_H
#include "ns3/mt-eventprocessor.h"
#include "QUIC-Frame.h"
#include <vector>
#include <utility>


namespace ns3
{

class QUICAppCommunication: public MTEventProcessor
{
public:
    QUICAppCommunication();
    EventProcessorOutput* Process(MTEvent* e, EventProcessorOutput* epOut);
    bool IsValidEvent(MTEvent* e);
};

} // namespace ns3

#endif