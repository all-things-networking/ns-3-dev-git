#ifndef QUIC_HANDLE_RECEIVE_ACK_H
#define QUIC_HANDLE_RECEIVE_ACK_H

#include <ctime> // std::time_t
#include <map>
#include <deque>
#include "QUIC_TEL-Frame.h"
#include "QUIC_TEL-Stream.h"
#include "QUIC_TEL-Iterm_out.h"

#include "../model/QUIC_TEL-Context.h"
#include "../model/QUIC_TEL-Event.h"
#include "ns3/mt-event.h"
#include "ns3/mt-eventprocessor.h"
#include "ns3/mt-state.h"
#include "ns3/node.h"
namespace ns3
{

class QUICFrame;
class Packet;

/**
 * \brief The class for a QUIC HandleRecieveACK
 */
// QUIC HandleRecieveACK
class QUICAckProcessor
{
public:
    QUICAckProcessor();
    ~QUICAckProcessor();

    void Process(AckEvent* ev, QUICContext *ctx, std::vector<QUICEvent *> events, std::vector<Packet *> packets, iterm_out *out);
    bool IsValidEvent(MTEvent * e);
};

} // namespace ns3

#endif
