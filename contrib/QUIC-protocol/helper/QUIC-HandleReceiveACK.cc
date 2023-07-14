#include "QUIC-HandleReceiveACK.h"
#include "QUIC-Frame.h"
#include "ns3/mt-eventprocessor.h"
#include <ctime>
#include <map>

namespace ns3
{

QUICHandleReceiveACK::QUICHandleReceiveACK()
{
}

QUICHandleReceiveACK::~QUICHandleReceiveACK()
{
}

bool QUICHandleReceiveACK::IsValidEvent(MTEvent * e)
{
    return true;
}

EventProcessorOutput*
QUICHandleReceiveACK::Process(MTEvent* e, EventProcessorOutput* epOut)
{
    MTContext * context = epOut->context;

    // I call mt->SendPack here
    QUICContext* c = dynamic_cast<QUICContext*>(context);
    ResponseEvent* responseEvent = dynamic_cast<ResponseEvent*>(e);

   // Update the state of the sent packet to ACKED
    ResponseEventData responseData = responseEvent->data;
    std::deque<std::pair<Ptr<Packet>, PacketState>> sentPackets = c->sentPackets;

    // ACKed packed be in range [sendBase, sendBase + windowSize]
    if (responseData.packetNum >= c->sendBase && responseData.packetNum <= c->sendBase + c->windowSize) {
        if (responseData.packetNum == c->sendBase) {
            c->sendBase += 1;
            c->sentPackets.pop_front();
        }
        else {
            sentPackets.at(responseData.packetNum).second = PacketState::ACKED;
        }
    }

    /*
    [RFC 9002]
    [https://www.rfc-editor.org/rfc/rfc9002#name-packet-threshold]

    Every packetNum that is between k_packet_threshold from current acked packet is considered lost
    */

    int lostIndex = responseData.packetNum - c->k_packet_threshold - 1;
    while (lostIndex > c->sendBase) {
        Ptr<Packet> lostPacket = sentPackets.at(responseData.packetNum).first;
        // WIP, need to seperate into data again and add the data to head of data buffers for retransmission

        uint8_t* buffer = new uint8_t[lostPacket->GetSize()];
        int size = lostPacket->CopyData(buffer, lostPacket->GetSize());
        std::string s = std::string(buffer, buffer + lostPacket->GetSize());

        std::cout << "Retransmitting: " << s << std::endl;

        std::stringstream retransmitPacket(s);
        std::string frame;

        while(std::getline(retransmitPacket, frame, '_'))
        {
            QUICFrame* dataFrame = new QUICFrame(frame);
            StreamFrameFields* streamFrameFields = static_cast<StreamFrameFields*>(dataFrame->fields);
            QUICStream* stream = c->quic_streams[streamFrameFields->StreamID];

            // Add to front of stream data buffer
            stream->databuffer = dataFrame->GetData() + stream->databuffer;
        }
    }
    

    // Empty Event
    std::vector<MTEvent*> newEvents;
    std::vector<Packet> packetTobeSend;

    // Output
    EventProcessorOutput* Output = new EventProcessorOutput;
    Output->newEvents = newEvents;
    Output->context = c;
    Output->packetToSend = packetTobeSend;
    return Output;
}


} // namespace ns3
