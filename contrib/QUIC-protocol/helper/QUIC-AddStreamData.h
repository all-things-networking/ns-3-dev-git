#ifndef QUIC_ADD_STREAM_DATA_H
#define QUIC_ADD_STREAM_DATA_H

#include <ctime> // std::time_t
#include <map>
#include <deque>
#include "QUIC-Frame.h"
#include "QUIC-StreamHandler.h"
#include "QUIC-Stream.h"
#include "QUIC-AddStreamData.h"

#include "../model/QUIC-Context.h"
#include "../model/QUIC-Event.h"
#include "ns3/mt-event.h"
#include "ns3/mt-eventprocessor.h"
#include "ns3/mt-state.h"
#include "ns3/node.h"
namespace ns3
{

class QUICFrame;
class Packet;

/**
 * \brief The class for a QUIC AddStreamData
 */
// QUIC AddStreamData
class QUICAddStreamData
{
public:
    QUICAddStreamData();
    ~QUICAddStreamData();

    EventProcessorOutput* TryAddStreamData(StreamEvent* e, QUICContext* c);
};

} // namespace ns3

#endif
