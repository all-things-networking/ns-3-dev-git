#ifndef MT_EVENTPROCESSOR_H
#define MT_EVENTPROCESSOR_H

#include <vector>
#include <utility> // std::pair

//mostly copied from eric's branch, though send if possible is mine
namespace ns3
{

class MtEvent;
class MtContext;

/**
 * \brief The base class for event processor. This is virtual class.
 */
class MtEventProcessor
{
public:
    MtEventProcessor();

    /**
     * \brief Process the event and return the processed results.
     * \param e The input event to be processed.
     * \param c The context of the Mt connection.
     * \return The modified context and new generated event if exits.
     */
    virtual std::pair<std::vector<MtEvent>, MtContext> Process(MtEvent e, MtContext c) = 0;


    /**
     * \brief Check if the input event is valid event type for the processor.
     * \param e The input event to be processed.
     * \return True if input event type matches the processor type, false otherwise.
     */
    virtual bool IsValidEvent(MtEvent e) = 0;
};


/**
 * \brief The processor handling the acknowledgement events.
 */
class TcpAckHandler: public MtEventProcessor
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
    std::pair<std::vector<MtEvent>, MtContext> Process(MtEvent e, MtContext c);


    /**
     * \brief Check if the input event is valid event type for the processor.
     * \param e The input event to be processed.
     * \return True if input event type matches the processor type, false otherwise.
     */
    bool IsValidEvent(MtEvent e);
};

/**
 * \brief The processor managing congestion control algorithms.
*/
class TcpCongControl: public MtEventProcessor
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
    std::pair<std::vector<MtEvent>, MtContext> Process(MtEvent e, MtContext c);

    /**
     * \brief Check if the input event is valid event type for the processor.
     * \param e The input event to be processed.
     * \return True if input event type matches the processor type, false otherwise.
     */
    bool IsValidEvent(MtEvent e);
};

class SendIfPossible: public MtEventProcessor
{
    vector <Packets> packetTobeSend;
public:
    SendIfPossible();
    std::pair<std::vector<MtEvent>, MtContext> Process(MtEvent e, MtContext c);
    vector<Packets> getPackets();
    bool IsValidEvent(MtEvent e);
}

} // namespace ns3

#endif
