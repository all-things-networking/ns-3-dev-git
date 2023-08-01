#ifndef QUIC_PACKET_DEMULTIPLEXER_H
#define QUIC_PACKET_DEMULTIPLEXER_H
#include "ns3/mt-eventprocessor.h"
#include "QUIC-Frame.h"
#include "QUIC-Header.h"
#include "../model/QUIC-Context.h"
#include <vector>
#include <utility>


namespace ns3
{

class QUICPacketDemultiplexer: public MTEventProcessor
{
private:
    Packet* recvPacket;
    // std::vector<std::pair<uint64_t, QUICFrame>> FrameToStream;
    
    // az new structure pair<header, data>
    std::vector<std::pair<QUICFrameHeader, std::string>> FrameToStream;
    // std::vector<QUICFrame> FrameToStream;

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

} // namespace ns3

#endif