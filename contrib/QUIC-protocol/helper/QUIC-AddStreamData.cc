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

bool QUICAddStreamData::IsValidEvent(MTEvent * e)
{
    return true;
}


EventProcessorOutput*
QUICAddStreamData::Process(MTEvent* e, EventProcessorOutput* epOut)
{
    MTContext * context = epOut->context;

    // I call mt->SendPack here
    QUICContext* c = dynamic_cast<QUICContext*>(context);
    StreamEvent* streamEvent = dynamic_cast<StreamEvent*>(e);

    int stream_id = streamEvent->stream_id;

    // Check if stream (stream_id) exists, if not create it
    auto it = c->quic_streams.find(stream_id);
    if (it == c->quic_streams.end())
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
            stream_id = c->quic_streams.size() + 1;
        }

        QUICStream* stream = new QUICStream(stream_id);
        c->quic_streams[stream_id] = stream;
        std::cout << "Created new stream with ID: "
                    << c->quic_streams.find(stream_id)->second->id << std::endl;
    }

    QUICStream* stream = c->quic_streams[stream_id];
    stream->AddToDataBuffer(streamEvent->data.text);

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
