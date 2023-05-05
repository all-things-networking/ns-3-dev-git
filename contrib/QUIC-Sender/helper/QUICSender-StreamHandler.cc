#include "QUICSender-StreamHandler.h"
#include "QUICSender-Stream.h"

#include "../model/QUICSender-Context.h"
#include "../model/QUICSender-Event.h"

#include "ns3/mt-event.h"
#include "ns3/mt-eventprocessor.h"
#include "ns3/mt-state.h"
#include "ns3/node.h"

#include <algorithm> // std::min, std::max
#include <iostream>
#include <utility> // std::pair
#include <vector>

namespace ns3
{

// TODO: This file needs to be adapted for QUIC.

class ModularTransport;
class StreamEvent;

// Send if possible, mine
QUICStreamHandler::QUICStreamHandler()
    : MTEventProcessor()
{
}

bool
QUICStreamHandler::IsValidEvent(MTEvent * e)
{
    return true;
}

EventProcessorOutput*
QUICStreamHandler::Process(MTEvent* e, EventProcessorOutput* epOut)
{
    MTContext * c = epOut->context;

    // I call mt->SendPack here
    QUICContext* newContext = dynamic_cast<QUICContext*>(c);
    StreamEvent* streamEvent = dynamic_cast<StreamEvent*>(e);

    // If we have a SEND_PACKET event we will send the packet
    if (streamEvent->streamEventType == StreamEventType::SEND_PACKET)
    {
        EventProcessorOutput* res = TrySendPacket(streamEvent, newContext);
        return res;
    }

    // If we have a ADD_DATA event, create a dataFrame and send it
    if (streamEvent->streamEventType == StreamEventType::ADD_DATA)
    {
        int stream_id = streamEvent->stream_id;

        // Check if stream (stream_id) exists, if not create it
        auto it = newContext->quic_streams.find(stream_id);
        if (it == newContext->quic_streams.end())
        {
            // We have no stream id provided so we choose one
            if (stream_id == NO_STREAM_ID)
            {
                /*
                [RFC 9000]
                Before a stream is created, all streams of the same type with lower-numbered stream
                IDs MUST be created. This ensures that the creation order for streams is consistent
                on both endpoints.”
                */
                stream_id = newContext->quic_streams.size() + 1;
            }

            QUICStream* stream = new QUICStream(stream_id);
            newContext->quic_streams[stream_id] = stream;
            std::cout << "Created new stream with ID: "
                      << newContext->quic_streams.find(stream_id)->second->id << std::endl;
        }

        QUICStream* stream = newContext->quic_streams[stream_id];
        stream->AddToDataBuffer(streamEvent->data.text);
    }

    // Empty Event
    std::vector<MTEvent*> newEvents;
    std::vector<Packet> packetTobeSend;

    // Output
    EventProcessorOutput* Output = new EventProcessorOutput;
    Output->newEvents = newEvents;
    Output->context = newContext;
    Output->packetToSend = packetTobeSend;
    return Output;
}

EventProcessorOutput*
QUICStreamHandler::TrySendPacket(StreamEvent* e, QUICContext* c)
{
    QUICPacketBuffer* PacketBuffer = c->PacketBuffer;

    std::vector<MTEvent*> newEvents;
    std::vector<Packet> packetTobeSend;
    EventProcessorOutput* Output = new EventProcessorOutput;

    // [RFC 9000] - Stream prioritization and multiplexing are left to the application.
    // For now just do round robin

    // If buffer CAN'T fit our dataFrame we will create a packet and create a
    // SendPacketEvent

    while (PacketBuffer->frames.size() != 4)
    {
        // For now just do round robin
        auto it = c->quic_streams.begin();
        std::advance(it, c->CurrentStream);
        QUICStream* stream = it->second;

        // TODO: this way of iterating is super inefficient. We should optimize this later
        size_t streamCount = 0;
        while (stream->databuffer.size() == 0 && streamCount < c->quic_streams.size())
        {
            c->CurrentStream = (c->CurrentStream + 1) % c->quic_streams.size();
            it = c->quic_streams.begin();
            std::advance(it, c->CurrentStream);
            stream = it->second;
            streamCount += 1;
        }

        // None of our streams have data left to be sent 
        if (streamCount >= c->quic_streams.size()) {
            break;
        }

        // Current stream has some data in databuffer, we should create a frame 
        std::string curr_data = stream->databuffer;
        
        //////////////////// Create a frame of size MAX_STREAM_DATA ////////////////////
        int MAX_STREAM_DATA = 5;
        QUICFrame* dataFrame = new QUICFrame;

        // Headers/fields for the frame
        StreamFrameFields * streamFrameFields = new StreamFrameFields;
        streamFrameFields->StreamID = c->CurrentStream;
        
        dataFrame->type = FrameType::STREAM;
        dataFrame->fields = streamFrameFields;

        // The "-" is used to seperate header and "_" is used to seperate frames
        std::string substrWithHeader = dataFrame->generateHeader() + "-" + curr_data.substr(0, MAX_STREAM_DATA) + "_";

        Ptr<Packet> data = Create<Packet>(reinterpret_cast<const uint8_t*>(substrWithHeader.data()), substrWithHeader.size());
        dataFrame->data = data;
        /////////////////////////////////////////////////////////////////////////////////

        // We need to update the buffer by erasing the first MAX_STREAM_DATA characters
        stream->databuffer = curr_data.erase(0, MAX_STREAM_DATA);

        // Add the frame to the packet buffer
        PacketBuffer->AddFrame(dataFrame);
        c->CurrentStream = (c->CurrentStream + 1) %
                           c->quic_streams.size(); // Move onto next stream for round robin
    }

    std::cout << "Creating packet and sending" << std::endl;
    Ptr<Packet> pkt = PacketBuffer->CreatePacket();
    std::pair<Ptr<Packet>, PacketState> sent = std::make_pair(PacketBuffer->CreatePacket(), PacketState::SENT);
    c->sentPackets.push_back(sent);

    // TODO: right now this only outputs a new EventProcessorOutput for chaining
    // but maybe we want to send the SendPacketEvent to the queue instead. In that case
    // we need to change this to add to queue intead

    // Technically we are already sending the packet when we add to packetTobeSend so we
    // don't need to creata new event called SEND_PACKET MTEvent* sendPacketEvent = new
    // StreamEvent(e->flow_id, StreamEventType::SEND_PACKET, 0); // stream_id doesn't
    // really matter newEvents.emplace_back(sendPacketEvent);

    Packet P = *pkt;
    packetTobeSend.emplace_back(P);

    Output->newEvents = newEvents;
    Output->context = c;
    Output->packetToSend = packetTobeSend;
    return Output;
}

} // namespace ns3
