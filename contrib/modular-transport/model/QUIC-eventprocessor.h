#ifndef MT_QUIC_EVENT_PROCESSOR_H
#define MT_QUIC_EVENT_PROCESSOR_H
#include "mt-eventprocessor.h"
#include "QUIC-Frame.h"
#include <vector>
#include <utility>


namespace ns3
{

class QUICAppCommunication: public MTEventProcessor
{
public:
    QUICAppCommunication();
    EventProcessorOutput* Process(MTEvent* e, EventProcessorOutput* epOut);
    bool IsValidEvent(MTEvent* e);
};

class QUICPacketDemultiplexer: public MTEventProcessor
{
private:
    Packet* recvPacket;
    std::vector<std::pair<uint64_t, QUICFrame>> FrameToStream;

    /**
     * Function for de-multiplexing the received packet into frames
     * TODO: implement this
    */
    void demultiplexePacket();
public:
    QUICPacketDemultiplexer();
    ~QUICPacketDemultiplexer();

    /**
     * Process the event.
     * Note for epOut, the IntermediateOutput should contain a mapping of frames to their streams
     *      to be processed by BufferManagement
    */
    EventProcessorOutput* Process(MTEvent* e, EventProcessorOutput* epOut);
    bool IsValidEvent(MTEvent* e);
};


class QUICBufferManagement: public MTEventProcessor
{
public:
    QUICBufferManagement();
    EventProcessorOutput* Process(MTEvent* e, EventProcessorOutput* epOut);
    bool IsValidEvent(MTEvent* e);
};

/**
 * \brief The processor handling the acknowledgement events.
 */
class QUICAckHandler: public MTEventProcessor
{
public:
    QUICAckHandler();
    EventProcessorOutput* Process(MTEvent* e, EventProcessorOutput* epOut);
    bool IsValidEvent(MTEvent* e);
};


} // namespace ns3

#endif