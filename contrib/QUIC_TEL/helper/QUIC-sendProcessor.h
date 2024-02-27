#ifndef QUIC_SEND_PACKET_H
#define QUIC_SEND_PACKET_H

#include <ctime> // std::time_t
#include <map>
#include <deque>
#include "QUIC_TEL-Frame.h"
#include "QUIC-StreamHandler.h"
#include "QUIC-Stream.h"
#include "QUIC_TEL-Iterm_out.h"

#include "../model/QUIC-Context.h"
#include "../model/QUIC-Event.h"
#include "ns3/mt-event.h"
#include "ns3/mt-eventprocessor.h"
#include "ns3/mt-state.h"
#include "ns3/node.h"
namespace ns3
{

class QUICFrame;
class QUICPacketBuffer;
class Packet;

/**
 * \brief The class for a QUICSendProcessor
 */
// QUICSendProcessor
class QUICSendProcessor : public MTEventProcessor
{
public:
    QUICSendProcessor();
    ~QUICSendProcessor();

    void Process(QUICEvent* e, QUICContext *ctx, std::vector<QUICEvent *> events, std::vector<Packet *> packets, iterm_out *out);
    bool IsValidEvent(MTEvent * e);
};

} // namespace ns3

#endif
