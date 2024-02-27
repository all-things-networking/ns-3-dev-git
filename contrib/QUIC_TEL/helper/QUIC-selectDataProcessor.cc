#include "QUIC-selectDataProcessor.h"

#include "../model/QUIC-Context.h"
#include "../model/QUIC-Event.h"
#include "QUIC_TEL-QuicHeader.h"
#include "QUIC-Stream.h"
#include "QUIC_TEL-Frame.h"

#include "ns3/mt-eventprocessor.h"

#include <algorithm> // added for min function
#include <chrono>
#include <ctime>
#include <map>

namespace ns3
{

    selectDataProcessor::selectDataProcessor()
    {
    }

    selectDataProcessor::~selectDataProcessor()
    {
    }

    void
    selectDataProcessor::own_Process(SendEvent* e,
                                QUICContext ctx,
                                std::vector<QUICEvent*> events,
                                iterm_out out)
    {
        int sent_size = 0;
        int i = 0;
        Packet pkt;
        QUICStream qs = QUICStream(0);         // TODO: better definition of QUICStream
        int bytes_allowed = ctx.congestion_window - ctx.bytes_in_flight;
        int frame_size_limit = bytes_allowed / ctx.streams.size(); // len changed to size here
        while (bytes_allowed > 0)
        {
            PacketInfo pkt_info;
            while (sent_size < ctx.size_limit && i < ctx.streams.size() && bytes_allowed > 0)   {
                qs = *ctx.streams[i];
                Frame frame = Frame(0,0,0); // TODO: Frame need redefinition
                pkt_info.stream_id.emplace_back(i); // add changed to emplace_back
                auto min_value = std::min({qs.frame_size_limit,
                                        bytes_allowed,
                                        frame_size_limit,
                                        static_cast<int>(qs.data.size()) - qs.last_sent}); // TODO: change the usage of this MIN
                pkt_info.frame_size.emplace_back(frame.size); // add changed to emplace_back, is Frame.size just the data_length?
                pkt_info.start_point.emplace_back(qs.last_sent);   // add changed to emplace_back

                frame.data = std::vector<char>(qs.data.begin() + qs.last_sent, 
                                                qs.data.begin() + qs.last_sent + frame.size); // frame.data = qs.data [qs.last_sent:qs.last_sent + frame.size];

                qs.last_sent = qs.last_sent + frame.size;
                Packet pkt = Packet(reinterpret_cast<uint8_t*> (&frame), sizeof(reinterpret_cast<uint8_t*> (&frame))); // add std:: before byte
                ctx.bytes_in_flight = ctx.bytes_in_flight + frame.data.size(); // len to size
                bytes_allowed = bytes_allowed - frame.data.size(); // len to size
                i = i + 1;
            }
            pkt_info.time_sent = std::chrono::system_clock::now(); // change to from now()
            QuicHeader qheader; //Quic Header not defined yet
            qheader.pkt_id = ctx.id_counter + 1;
            pkt_info.packet_id = qheader.pkt_id;
            pkt.add_hdr(qheader);
            ctx.sent_packets.emplace_back(pkt_info); // add to emplace_back
            //tx_module.add(pkt); // pick a queue to add a packet (this add should be a builtin function for tx_module class)
        }
    }
} // namespace ns3
