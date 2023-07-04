#include "QUIC-AddStreamData.h"
#include "QUIC-Frame.h"
#include "ns3/mt-eventprocessor.h"
#include <ctime>
#include <map>

namespace ns3
{

QUICAddStreamData::QUICAddStreamData()
{
}

QUICAddStreamData::~QUICAddStreamData()
{
}

EventProcessorOutput*
QUICAddStreamData::TryAddStreamData(StreamEvent* streamEvent, QUICContext* newContext)
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


} // namespace ns3
