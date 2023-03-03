#ifndef MT_QUIC_EVENT_PROCESSOR_H
#define MT_QUIC_EVENT_PROCESSOR_H
#include "mt-eventprocessor.h"
#include "QUICPacket.h"
#include "QUICFrame.h"
#include <vector>
#include <utility>


namespace ns3
{

class QUICAppCommunication: public MTEventProcessor
{
public:
    QUICAppCommunication();

    /**
     * \brief Process the event and return the processed results.
     * \param e The input event to be processed.
     * \param c The context of the Mt connection.
     * \return The modified context and new generated event if exits.
     *
     */
    EventProcessorOutput* Process(MTEvent e, EventProcessorOutput epOut);


    /**
     * \brief Check if the input event is valid event type for the processor.
     * \param e The input event to be processed.
     * \return True if input event type matches the processor type, false otherwise.
     */
    bool IsValidEvent(MTEvent e);
};

class QUICPacketDemultiplexer: public MTEventProcessor
{
private:
    QUICPacket recvPacket;
    std::vector<std::pair<uint64_t, QUICFrame>> FrameToStream;

    void demultiplexePacket();
    void processFrame(EventProcessorOutput epOut);
public:
    QUICPacketDemultiplexer();

    /**
     * \brief Process the event and return the processed results.
     * \param e The input event to be processed.
     * \param c The context of the Mt connection.
     * \return The modified context and new generated event if exits.
     *
     */
    EventProcessorOutput* Process(MTEvent e, EventProcessorOutput epOut);


    /**
     * \brief Check if the input event is valid event type for the processor.
     * \param e The input event to be processed.
     * \return True if input event type matches the processor type, false otherwise.
     */
    bool IsValidEvent(MTEvent e);
};


class QUICBufferManagement: public MTEventProcessor
{
public:
    QUICBufferManagement();

    /**
     * \brief Process the event and return the processed results.
     * \param e The input event to be processed.
     * \param c The context of the Mt connection.
     * \return The modified context and new generated event if exits.
     *
     */
    EventProcessorOutput* Process(MTEvent e, EventProcessorOutput epOut);


    /**
     * \brief Check if the input event is valid event type for the processor.
     * \param e The input event to be processed.
     * \return True if input event type matches the processor type, false otherwise.
     */
    bool IsValidEvent(MTEvent e);
};

/**
 * \brief The processor handling the acknowledgement events.
 */
class QUICAckHandler: public MTEventProcessor
{
public:
    QUICAckHandler();

    /**
     * \brief Process the event and return the processed results.
     * \param e The input event to be processed.
     * \param c The context of the Mt connection.
     * \return The modified context and new generated event if exits.
     *
     */
    EventProcessorOutput* Process(MTEvent e, EventProcessorOutput epOut);


    /**
     * \brief Check if the input event is valid event type for the processor.
     * \param e The input event to be processed.
     * \return True if input event type matches the processor type, false otherwise.
     */
    bool IsValidEvent(MTEvent e);
};


} // namespace ns3

#endif