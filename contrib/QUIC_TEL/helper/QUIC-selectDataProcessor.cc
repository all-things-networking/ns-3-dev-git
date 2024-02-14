#include "QUIC-selectDataProcessor.h"
#include "QUIC-Frame.h"
#include "../model/QUIC-Event.h"
#include "../model/QUIC-Context.h"
#include "ns3/mt-eventprocessor.h"
#include <ctime>
#include <map>

namespace ns3
{

selectDataProcessor::selectDataProcessor() {
}

selectDataProcessor::~selectDataProcessor() {
}

void selectDataProcessor::Process(SendEvent* e, QUICContext ctx, std::vector<QUICEvent*> events, interm_out out, tx_module tx) {
    int sent_size = 0;
    int i = 0;
    pkt_t pkt = new_pkt(); // packet
    QuicStream qs;
    int bytes_allowed = ctx.congestion_window - ctx.bytes_in_flight;
    int frame_size_limit = bytes_allowed / ctx.streams.size(); // len changed to size here
    while (bytes_allowed > 0)
    {
        PacketInfo pkt_info;
        while (sent_size < ctx.size_limit && i < ctx.streams.size() && bytes_allowed > 0)
        {
            qs = ctx.streams[i];
            Frame frame;
            pkt_info.stream_id.emplace_back(i); // add changed to emplace_back
            frame.size = std::min(qs.frame_size_limit,
                             bytes_allowed,
                             frame_size_limit,
                             qs.data.len() - qs.last_sent);
            pkt_info.frame_size.emplace_back(frame.size); // add changed to emplace_back
            pkt_info.start_point.emplace(qs.last_sent); //add changed to emplace_back
            frame.data = qs.data [qs.last_sent:qs.last_sent + frame.size];
            qs.last_sent = qs.last_sent + frame.size;
            pkt.add_data(byte(frame));
            ctx.bytes_in_flight = ctx.bytes_in_flight + frame.data.len();
            bytes_allowed = bytes_allowed - frame.data.len();
            i = i + 1;
        }

        ///////////////////////////////////////////////////////////////////////////////////
        MTContext* context = epOut->context;

        // I call mt->SendPack here
        QUICContext* c = dynamic_cast<QUICContext*>(context);
        StreamEvent* streamEvent = dynamic_cast<StreamEvent*>(e);

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
            if (streamCount >= c->quic_streams.size())
            {
                break;
            }

            // Current stream has some data in databuffer, we should create a frame
            std::string curr_data = stream->databuffer;

            //////////////////// Create a frame of size MAX_STREAM_DATA ////////////////////
            int MAX_STREAM_DATA = 5;
            QUICFrame* dataFrame = new QUICFrame;

            // Headers/fields for the frame
            StreamFrameFields* streamFrameFields = new StreamFrameFields;
            streamFrameFields->StreamID = c->CurrentStream;

            dataFrame->type = FrameType::STREAM;
            dataFrame->fields = streamFrameFields;

            // The "-" is used to seperate header and "_" is used to seperate frames
            std::string substrWithHeader =
                dataFrame->generateHeader() + "-" + curr_data.substr(0, MAX_STREAM_DATA) + "_";

            Ptr<Packet> data =
                Create<Packet>(reinterpret_cast<const uint8_t*>(substrWithHeader.data()),
                               substrWithHeader.size());
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
        std::pair<Ptr<Packet>, PacketState> sent =
            std::make_pair(PacketBuffer->CreatePacket(), PacketState::SENT);
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
