#include "QUIC-SendPacket.h"
#include "QUIC-Frame.h"
#include "ns3/mt-eventprocessor.h"
#include <ctime>
#include <map>

namespace ns3
{

QUICSendPacket::QUICSendPacket()
{
}

QUICSendPacket::~QUICSendPacket()
{
}

bool QUICSendPacket::IsValidEvent(MTEvent * e)
{
    return true;
}


EventProcessorOutput*
QUICSendPacket::Process(QUICEvent* e, QuicContext *ctx, vector<QUICEvent *> events, vector<Packet *> packets, iterm_out out)
{
    if (out.info.in_flight) {
        if (out.info.ack_eliciting) {
            ctx.time_of_last_ack_elicting_packet = time(0);
    }
}


} // namespace ns3
