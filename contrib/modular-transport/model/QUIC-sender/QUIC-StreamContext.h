#ifndef QUIC_STREAM_CONTEXT_H
#define QUIC_STREAM_CONTEXT_H

#include <ctime> // std::time_t
#include <map>
namespace ns3
{

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
class QUICStreamContext
{
public:
    QUICStreamContext(int id);
    ~QUICStreamContext();

    // Self defined
    QUICStreamState state = QUICStreamState::NONE; // Current sender stream state
    uint32_t id; // ID of the stream
    uint32_t offset; // Offset for frame
};

} // namespace ns3

#endif
