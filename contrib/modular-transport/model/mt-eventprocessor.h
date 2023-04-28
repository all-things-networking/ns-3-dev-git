#ifndef MT_EVENTPROCESSOR_H
#define MT_EVENTPROCESSOR_H

#include <vector>
#include <utility> // std::pair
#include "mt-intermediate_output.h"

namespace ns3
{

class MTEvent;
class MTContext;
class Packet;
struct EventProcessorOutput{
     std::vector<MTEvent*> newEvents;
     MTContext* context;
     std::vector<Packet> packetToSend;
     IntermediateOutput intermOutput;
 };
/**
 * \brief The base class for event processor. This is virtual class.
 */
struct EventProcessorOutput;
class MTEventProcessor
{
public:
    MTEventProcessor();

    /**
     * \brief Process the event and return the processed results.
     * \param e The input event to be processed.
     * \param c The context of the Mt connection.
     * \return The modified context and new generated event if exits.
     */
    virtual EventProcessorOutput* Process(MTEvent* e, EventProcessorOutput* epOut) = 0;


    /**
     * \brief Check if the input event is valid event type for the processor.
     * \param e The input event to be processed.
     * \return True if input event type matches the processor type, false otherwise.
     */
    virtual bool IsValidEvent(MTEvent* e) = 0;
};


/**
 * \brief The processor handling the acknowledgement events.
 */
class TcpAckHandler: public MTEventProcessor
{
public:
    TcpAckHandler();

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
    EventProcessorOutput* Process(MTEvent* e, MTContext c);


    /**
     * \brief Check if the input event is valid event type for the processor.
     * \param e The input event to be processed.
     * \return True if input event type matches the processor type, false otherwise.
     */
    bool IsValidEvent(MTEvent* e);
};

} // namespace ns3

#endif
