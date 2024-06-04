#include "../model/QUIC_TEL-Context.h"
#include "../model/QUIC_TEL-Event.h"
#include "QUIC_TEL-Frame.h"
#include "QUIC_TEL-Stream.h"
#include "QUIC_TEL-Iterm_out.h"

#include "ns3/mt-event.h"
#include "ns3/mt-eventprocessor.h"
#include "ns3/mt-state.h"
#include "ns3/node.h"
#include "ns3/simulator.h"

#include <deque>
#include <map>
#include <vector>

namespace ns3
{

class QUICFrame;
class QUICPacketBuffer;
class Packet;

/**
 * \brief The class for a QUIC selectDataProcessor
 */
// QUIC selectDataProcessor
class selectDataProcessor : public MTEventProcessor
{
public:
    selectDataProcessor();
    ~selectDataProcessor();
    EventProcessorOutput* Process(MTEvent *e, EventProcessorOutput *epOut);
    bool IsValidEvent(MTEvent* e);

    // Own definition of the processor
};

} // namespace ns3
