#ifndef QUIC_BUFFER_MANAGEMENT_H
#define QUIC_BUFFER_MANAGEMENT_H
#include "ns3/mt-eventprocessor.h"
#include "QUIC-Frame.h"
#include <vector>
#include <utility>


namespace ns3
{

class QUICBufferManagement: public MTEventProcessor
{
public:
    QUICBufferManagement();
    EventProcessorOutput* Process(MTEvent* e, EventProcessorOutput* epOut);
    bool IsValidEvent(MTEvent* e);
};

} // namespace ns3

#endif