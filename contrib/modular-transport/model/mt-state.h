#ifndef MT_STATE_H
#define MT_STATE_H

#include <ctime> // std::time_t
#include <map>

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
class MtContext{
public:
    int saddr;
    int daddr;
    MtContext();

};
/**
 * \brief The class for maintaing the state variables for ONE Mt connections.
 */
// TCP context
class TcpContext: public MtContext
{
public:
    TcpContext(int tcpId);

    // rfc9293 - Sec 3.3.1
    uint32_t m_Una;           // send unacknowledged
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
    uint32_t m_dupThreshold;  // Threshold for triggler packet loss, usually 3
};

/**
 * \brief The class for maintaing the state variables for ALL Mt connections.
 *
 * Currently all contexts are stored on stack.
 * TODO: may move the data storage to heap using alloc or smart pointer.
 */
class MtState
{
public:
    MtState(ModularTransport* mt);

    /**
     * \brief Get the context of Mt connection with id.
     * \param id The id of Mt connection.
     * \return The context that belongs to the specific Mt connection.
     */
    TcpContext GetVal(int id);
    /**
     * \brief Store the context data to m_library, given Mt connection id.
     * \param id The id of Mt connection.
     * \param data The context data of the Mt.
     */
    void Write(int id, MtContext data);

private:
    ModularTransport* m_mt;
    std::map<int, TcpContext> m_library; // database of all Mt connnections.
};

} // namespace ns3

#endif
