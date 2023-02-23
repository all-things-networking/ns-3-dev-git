#ifndef QUIC_ACK_HANDLER_H
#define QUIC_ACK_HANDLER_H
#include "QUIC-Dispatcher.h"
#include "../QUIC-EventProcessor.h"

namespace ns3
{
class MTEvent;
class MTEventQueue;
class MTHeader;

/**
 * \brief The processor handling the acknowledgement events.
 */
class QUICAckHandler: public QUICEventProcessor
{
public:
    QUICAckHandler();

    /**
     * \brief Process the event and return the processed results.
     * \param e The input event to be processed.
     * \param c The context of the Mt connection.
     * \return The modified context and new generated event if exits.
     *
     * Discard the Ack Event when acknowledge number is invalid.
     * Active fact retrainsmit when duplicte Ack received.
     * Timeout lost should be handled by other event processors.
     * Perform Mt congestion control.
     */
    EventProcessorOutput* Process(MTEvent e, MTContext* c);


    /**
     * \brief Check if the input event is valid event type for the processor.
     * \param e The input event to be processed.
     * \return True if input event type matches the processor type, false otherwise.
     */
    bool IsValidEvent(MTEvent e);
};
} // namespace ns3
#endif
