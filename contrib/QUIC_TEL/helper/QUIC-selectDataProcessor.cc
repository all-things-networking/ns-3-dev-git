#include "QUIC-selectDataProcessor.h"

#include "../model/QUIC-Context.h"
#include "../model/QUIC-Event.h"
#include "QUIC-Stream.h"

#include "QUIC-Frame.h"

#include "ns3/mt-eventprocessor.h"

#include <algorithm> // added for min function
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
    selectDataProcessor::Process(SendEvent* e,
                                QUICContext ctx,
                                std::vector<QUICEvent*> events,
                                iterm_out out)
    {
        int sent_size = 0;
        int i = 0;
        Packet pkt = Packet(); // packet
        QUICStream qs = QUICStream(0);         // TODO: better definition of QUICStream
        int bytes_allowed = ctx.congestion_window - ctx.bytes_in_flight;
        int frame_size_limit = bytes_allowed / ctx.streams.size(); // len changed to size here
        while (bytes_allowed > 0)
        {
            PacketInfo pkt_info;
            while (sent_size < ctx.size_limit && i < ctx.streams.size() && bytes_allowed > 0)   {
                qs = ctx.streams[i];
                Frame frame; // TODO: Frame need redefinition
                pkt_info.stream_id.emplace_back(i); // add changed to emplace_back
                auto min_value = std::min({qs.frame_size_limit,
                                        bytes_allowed,
                                        frame_size_limit,
                                        static_cast<int>(qs.data.size()) - qs.last_sent}); // TODO: change the usage of this MIN
                pkt_info.frame_size.emplace_back(frame.size); // add changed to emplace_back
                pkt_info.start_point.emplace(qs.last_sent);   // add changed to emplace_back

                frame.data = std::vector<char>(qs.data.begin() + qs.last_sent, 
                                                qs.data.begin() + qs.last_sent + frame.size); // frame.data = qs.data [qs.last_sent:qs.last_sent + frame.size];

                qs.last_sent = qs.last_sent + frame.size;
                pkt.add_data(std::byte(frame));// add std:: before byte
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
