#ifndef QUIC_STREAM_HANDLER_H
#define QUIC_STREAM_HANDLER_H
#include "../mt-eventprocessor.h"
#include <stdint.h>
#include "QUIC-StreamHandler.h"

namespace ns3
{

class MTEvent;
class QUICContext;
class Packet;


/**
 * \brief The processor handling the stream events.
 */
class QUICStreamHandler: public MTEventProcessor
{
public:
    QUICStreamHandler();
    EventProcessorOutput* Process(MTEvent* e, MTContext* c);
    bool IsValidEvent(MTEvent e);

    // void Send(Packet* pkt, uint32_t streamId);
    // void Recv(Packet* pkt, uint32_t streamId);
    // void ChangeStreamState(uint32_t streamId);
    // uint32_t CreateStream(); // Creates QUIC stream and returns the ID
    // void DeleteStream(uint32_t streamId); // Creates QUIC stream and returns the ID
    
};
} // namespace ns3
#endif
