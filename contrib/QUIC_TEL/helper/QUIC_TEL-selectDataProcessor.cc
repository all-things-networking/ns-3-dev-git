#include "QUIC_TEL-selectDataProcessor.h"

#include "../model/QUIC_TEL-Context.h"
#include "../model/QUIC_TEL-Event.h"
#include "QUIC_TEL-QuicHeader.h"
#include "QUIC_TEL-Stream.h"
#include "QUIC_TEL-Frame.h"

#include "ns3/mt-eventprocessor.h"

#include <algorithm> // added for min function
#include <chrono>
#include <map>
#include <vector>

namespace ns3
{
    EventProcessorOutput* selectDataProcessor::Process(MTEvent* e, EventProcessorOutput* epOut) {
        return nullptr;
    }

    bool selectDataProcessor::IsValidEvent(MTEvent* e) {
        return true;
    }

    selectDataProcessor::selectDataProcessor()
    {
    }

    selectDataProcessor::~selectDataProcessor()
    {
    }

    EventProcessorOutput *
    selectDataProcessor::Process(MTEvent *e, EventProcessorOutput *epOut)
    {
        QUICContext *ctx = epOut->ctx;
        int sent_size = 0;
        int i = 0;
        Packet pkt;
        QUICStream *qs = new QUICStream(0);
        int bytes_allowed = ctx->congestion_window - ctx->bytes_in_flight;
        int frame_size_limit = bytes_allowed / ctx->streams.size(); // len changed to size here
        int frame_size_limit = max(bytes_allowed/ctx.streams.size(), bytes_allowed/ctx->max_stream_per_pkt)
        while (bytes_allowed > 0)
        {
            PacketInfo pkt_info;
            while (sent_size < ctx->size_limit && i < ctx->streams.size() && bytes_allowed > 0)   {
                qs = ctx->streams[i];
                Frame frame = Frame(0,0,0);
                pkt_info.stream_id.emplace_back(i);
                frame.size = std::min({qs->frame_size_limit,
                                        bytes_allowed,
                                        frame_size_limit,
                                        static_cast<int>(qs->data.size()) - qs->last_sent}); // TODO: change the usage of this MIN
                pkt_info.frame_size.emplace_back(frame.data_length); // add changed to emplace_back, is Frame.size just the data_length?
                pkt_info.start_point.emplace_back(qs->last_sent);   // add changed to emplace_back

                frame.data = std::vector<char>(qs->data.begin() + qs->last_sent, 
                                                qs->data.begin() + qs->last_sent + frame.data_length); // frame.data = qs->data [qs->last_sent:qs->last_sent + frame.size];

                qs->last_sent = qs->last_sent + frame.data_length;
                Packet pkt = Packet(reinterpret_cast<uint8_t*> (&frame), sizeof(reinterpret_cast<uint8_t*> (&frame))); // add std:: before byte
                ctx->bytes_in_flight = ctx->bytes_in_flight + frame.data.size(); // len to size
                bytes_allowed = bytes_allowed - frame.data.size(); // len to size
                i = i + 1;
            }
            pkt_info.time_sent = Simulator::Now();
            QuicHeader qheader = QuicHeader(ctx->id_counter + 1); //Init
            pkt_info.packet_id = qheader.pkt_id;
            pkt.AddHeader(qheader); // replace with correct name
            ctx->sent_packets.emplace_back(&pkt_info); // add to emplace_back
            //tx_module.add(pkt); // pick a queue to add a packet (this add should be a builtin function for tx_module class)
        }
    }
} // namespace ns3
