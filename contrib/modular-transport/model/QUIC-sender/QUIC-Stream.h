#ifndef QUIC_STREAM_H
#define QUIC_STREAM_H

#include <ctime> // std::time_t
#include <map>
#include <deque>
#include "QUIC-Frame.h"
namespace ns3
{

class QUICFrame;

// RFC9000 Section 3
enum QUICStreamState
{
    NONE,
    READY,
    SEND,
    DATA_SENT,
    RESET_SENT,
    DATA_RECVD,
    RESET_RECVD,
    ERROR
};

/**
 * \brief The class for maintaing the state variables for ONE QUIC stream.
 */
// QUIC context
class QUICStream
{
public:
    QUICStream(int id);
    ~QUICStream();

    // Self defined
    QUICStreamState state = QUICStreamState::NONE; // Current sender stream state
    uint32_t id; // ID of the stream
    uint32_t offset; // Offset for frame

    // Buffer frames associated for this stream\
    std::deque<QUICFrame*> frames;
    void AddFrame(QUICFrame* frame);
    QUICFrame* RemoveFrame();
    std::deque<QUICFrame*> frames;
};

} // namespace ns3

#endif
