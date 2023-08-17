#ifndef QUIC_BUFFER_MANAGEMENT_H
#define QUIC_BUFFER_MANAGEMENT_H
#include "ns3/mt-eventprocessor.h"
#include "QUIC-Frame.h"
#include <vector>
#include <utility>
#include <unordered_set>
#include "../model/QUIC-Context.h"
#include "QUIC-IntermediateOutput.h"


namespace ns3
{

class QUICBufferManagement: public MTEventProcessor
{
    void generateFlowControlFrames(
        QUICContext* qc, 
        std::unordered_set<int>& updatedStreamIDs,
        QUICIntermediateOutput* intermOutput);
public:
    QUICBufferManagement();
    EventProcessorOutput* Process(MTEvent* e, EventProcessorOutput* epOut);
    bool IsValidEvent(MTEvent* e);
};

} // namespace ns3

#endif