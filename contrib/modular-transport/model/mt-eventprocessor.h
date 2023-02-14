#ifndef MT_EVENTPROCESSOR_H
#define MT_EVENTPROCESSOR_H

#include <vector>
#include <utility> // std::pair

//mostly copied from eric's branch, though send if possible is mine
namespace ns3
{

class MTEvent;
class MTContext;
class Packet;
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
    virtual EventProcessorOutput Process(MTEvent e, MTContext c) = 0;


    /**
     * \brief Check if the input event is valid event type for the processor.
     * \param e The input event to be processed.
     * \return True if input event type matches the processor type, false otherwise.
     */
    virtual bool IsValidEvent(MTEvent e) = 0;
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
    EventProcessorOutput Process(MTEvent e, MTContext c);


    /**
     * \brief Check if the input event is valid event type for the processor.
     * \param e The input event to be processed.
     * \return True if input event type matches the processor type, false otherwise.
     */
    bool IsValidEvent(MTEvent e);
};

/**
 * \brief The processor managing congestion control algorithms.
*/
class TcpCongControl: public MTEventProcessor
{
public:
    TcpCongControl();

    /**
     * \brief Process the event and return the processed results.
     * \param e The input event to be processed.
     * \param c The context of the Mt connection.
     * \return The modified context and new generated event if exits.
     *
     * Perform Mt congestion control based on RFC5681.
     */
    EventProcessorOutput Process(MTEvent e, MTContext c);

    /**
     * \brief Check if the input event is valid event type for the processor.
     * \param e The input event to be processed.
     * \return True if input event type matches the processor type, false otherwise.
     */
    bool IsValidEvent(MTEvent e);
};

class SendIfPossible: public MTEventProcessor
{
public:
    SendIfPossible();
    EventProcessorOutput Process(MTEvent e, MTContext c);
    bool IsValidEvent(MTEvent e);
};

} // namespace ns3

#endif
