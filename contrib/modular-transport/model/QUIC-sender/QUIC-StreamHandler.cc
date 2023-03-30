#include "QUIC-StreamHandler.h"

#include "../QUIC-Context.h"
#include "../QUIC-Event.h"
#include "../mt-event.h"
#include "../mt-eventprocessor.h"
#include "../mt-state.h"
#include "QUIC-Stream.h"

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
QUICStreamHandler::IsValidEvent(MTEvent e)
{
    return true;
}

EventProcessorOutput*
QUICStreamHandler::Process(MTEvent* e, MTContext* c)
{
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

        QUICFrame* dataFrame = new QUICFrame;
        dataFrame->data = streamEvent->data;
        dataFrame->state = FrameState::NOT_SENT;
        stream->AddFrame(dataFrame);
    }

    // Empty Event
    std::vector<MTEvent*> newEvents;
    std::vector<Packet> packetTobeSend;

    // Output
    EventProcessorOutput* Output = new EventProcessorOutput;
    Output->newEvents = newEvents;
    Output->updatedContext = newContext;
    Output->packetToSend = packetTobeSend;
    return Output;
    // store packets to send as vector in class
    // call get packet to retrieve it later, and clear vector in class, use temp vector
}

EventProcessorOutput*
QUICStreamHandler::TrySendPacket(StreamEvent* e, QUICContext* c)
{
    QUICPacketBuffer* PacketBuffer = c->PacketBuffer;

    std::vector<MTEvent*> newEvents;
    std::vector<Packet> packetTobeSend;
    EventProcessorOutput* Output = new EventProcessorOutput;

    // [RFC 9000] - Stream prioritization and multiplexing are left to the application.
    // For now we will just iterate over the map of streams and add frames for the associated stream
    // TODO: this may lead to starvation for some streams so we should change this implementation in
    // the future

    // If buffer CAN'T fit our dataFrame we will create a packet and create a
    // SendPacketEvent
    while (PacketBuffer->frames.size() != 4)
    {
        // For now just do round robin
        auto it = c->quic_streams.begin();
        std::advance(it, c->CurrentStream);
        QUICStream* stream = it->second;

        // TODO: this way of iterating is super inefficient. We should optimize this later
        while (stream->frames.size() == 0)
        {
            c->CurrentStream = (c->CurrentStream + 1) % c->quic_streams.size();
            it = c->quic_streams.begin();
            std::advance(it, c->CurrentStream);
            stream = it->second;
        }

        QUICFrame* currFrame = stream->frames.front();
        stream->frames.pop_front();

        PacketBuffer->AddFrame(currFrame);
        c->CurrentStream = (c->CurrentStream + 1) %
                           c->quic_streams.size(); // Move onto next stream for round robin
    }

    std::cout << "Creating packet and sending" << std::endl;
    Ptr<Packet> pkt = PacketBuffer->CreatePacket();
    PacketBuffer->sent.push_back(pkt);

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
    Output->updatedContext = c;
    Output->packetToSend = packetTobeSend;
    return Output;
}

} // namespace ns3
