#ifndef TCP_CONTEXT_H
#define TCP_CONTEXT_H

#include <ctime> // std::time_t
#include <map>
#include "ns3/ipv4-address.h"
#include "mt-context.h"
#include "TCP-timer.h"
#include "TCP-scheduler.h"
namespace ns3
{
class ModularTransport;
/**
 * \brief The class indicate phase in congestion control algorithms.
*/
enum TcpCongPhase
{
    NONE,
    SLOW_START,
    CONG_AVOID,
    FAST_RETRAINS,
    FAST_RECOVER
};
class MTContext;
/**
 * \brief The class for maintaing the state variables for ONE Mt connections.
 */
// TCP context
class TCPContext: public MTContext
{
public:
    TCPContext(int tcpId);
    void SetTimer(int duration, MTScheduler* scheduler);
    ~TCPContext();
    // rfc9293 - Sec 3.3.1

    uint8_t* data;
    uint32_t m_Una;           // send unacknowledged
    //TODO: Una is start
    uint32_t m_start;         // Start of Window
    uint32_t m_Nxt;           // send next
    uint32_t m_Wnd;           // send window, in bytes
    uint32_t m_Iss;           // initial send sequence number

    // rfc5681 - Sec 2
    uint32_t m_cnwd;          // congestion window
    uint32_t m_Smss;          // Sender maximum segment size
    // rfc5681 - Sec 3
    uint32_t m_N;             // the number of previously un-acked bytes acked in the incoming ACK

    // Self defined
    TcpCongPhase m_congPhase; // Current Congestion Control Phase
    uint32_t m_dupSeq;        // the sequence number of duplicate packet
    uint32_t m_dupAmount;     // the amount for duplicate packet
    uint32_t m_ssthresh;      // slow start threshold
    //MtTimer m_time_out;       // time when timeout occurs
    uint32_t m_segmentsize;
    uint32_t m_dupThreshold;  // Threshold for triggler packet loss, usually 3
    TCPTimer RTOTimer;
};

} // namespace ns3

#endif
