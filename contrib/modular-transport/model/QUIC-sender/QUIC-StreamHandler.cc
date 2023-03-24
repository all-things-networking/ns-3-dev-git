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
    int stream_id = streamEvent->stream_id;

    // Check if stream (stream_id) exists, if not create it
    auto it = newContext->quic_streams.find(stream_id);
    if (it == newContext->quic_streams.end())
    {
        QUICStream* stream = new QUICStream(stream_id);
        newContext->quic_streams[stream_id] = stream;
        std::cout << "Created new stream with ID: "
                  << newContext->quic_streams.find(stream_id)->second->id << std::endl;
    }

    QUICStream* stream = newContext->quic_streams[stream_id];

    // If we have a ADD_DATA event, create a dataFrame and send it
    if (streamEvent->streamEventType == StreamEventType::ADD_DATA)
    {
        QUICFrame* dataFrame = new QUICFrame;
        dataFrame->data = streamEvent->data;
        dataFrame->state = FrameState::NOT_SENT;
        stream->AddFrame(dataFrame);

        EventProcessorOutput* res = TrySendPacket(streamEvent, newContext);
        return res;
    }

    // If we have a SEND_PACKET event we will send the packet
    if (streamEvent->streamEventType == StreamEventType::SEND_PACKET)
    {
        //////////////////// PLACEHOLDER CODE FOR NOW ////////////////////
        std::vector<MTEvent*> newEvents;

        // New Packets
        // Add window
        std::vector<Packet> packetTobeSend;
        Packet P = Packet(newContext->data, 4);
        packetTobeSend.emplace_back(P);

        // Output
        EventProcessorOutput* Output = new EventProcessorOutput;
        Output->newEvents = newEvents;
        Output->updatedContext = newContext;
        Output->packetToSend = packetTobeSend;
        return Output;
        //////////////////// PLACEHOLDER CODE FOR NOW ////////////////////
    }

    // A
    std::vector<MTEvent*> newEvents;

    // New Packets
    // Add window
    std::vector<Packet> packetTobeSend;
    Packet P = Packet(newContext->data, 4);
    // packetTobeSend.emplace_back(P);

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
    for (const auto& pair : c->quic_streams)
    {
        int streamId = pair.first;
        QUICStream* stream = pair.second;

        // For now just add all the frames for the current stream
        while (stream->frames.size() > 0)
        {
            QUICFrame* currFrame = stream->frames.front();
            stream->frames.pop_front();

            // If buffer CAN'T fit our dataFrame we will create a packet and create a
            // SendPacketEvent
            if (PacketBuffer->IsFull() || !PacketBuffer->CanAddFrame(currFrame) ||
                PacketBuffer->frames.size() == 2)
            {
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
            }

            PacketBuffer->AddFrame(currFrame);
        }
    }

    Output->newEvents = newEvents;
    Output->updatedContext = c;
    Output->packetToSend = packetTobeSend;
    return Output;
}

} // namespace ns3
