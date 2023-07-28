#ifndef QUIC_STREAM_H
#define QUIC_STREAM_H

#include <ctime> // std::time_t
#include <map>
#include <deque>
#include <vector>
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
    QUICStreamState state = QUICStreamState::NONE; // Current  stream state
    uint32_t id; // ID of the stream
    uint32_t offset; // Offset for frame
    std::string databuffer;

    /////////// receiver side ////////////
    bool fin = 0; // indicate if size is known or not
    int size; // only valid if fin == 1
    std::vector<bool> is_received; // array indicate if a byte is received or not

    // Buffer should be better
    void AddToDataBuffer(std::string data);
    void AddToDataBuffer(std::string data, unsigned long start);
    void SetFin(bool finBit);
};

} // namespace ns3

#endif
