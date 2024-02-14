#ifndef QUIC_HANDLE_RECEIVE_ACK_H
#define QUIC_HANDLE_RECEIVE_ACK_H

#include <ctime> // std::time_t
#include <map>
#include <deque>
#include "QUIC-Frame.h"
#include "QUIC-StreamHandler.h"
#include "QUIC-Stream.h"
#include "QUIC_TEL-iterm_out.h"

#include "../model/QUIC-Context.h"
#include "../model/QUIC-Event.h"
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

    void Process(AckEvent* ev, QuicContext *ctx, vector<QUICEvent *> events, vector<Packet *> packets, iterm_out *out);
    bool IsValidEvent(MTEvent * e);
};

} // namespace ns3

#endif
