#ifndef QUIC_STREAM_H
#define QUIC_STREAM_H

#include <ctime> // std::time_t
#include <map>
#include <deque>
#include <vector>
#include "QUIC_TEL-Frame.h"
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

class QUICStream 
{
    public:
        QUICStream(int id);
        ~QUICStream();

        std::vector<char> data;
        int id;
        int frame_size_limit;
        int last_sent;
        std::vector<bool> acked_frames; // list<bool> acked_frames
};

}
#endif
