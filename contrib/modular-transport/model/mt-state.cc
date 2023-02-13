#include "mt-state.h"

#include "mt-timer.h"
#include <ctime>
#include <map>

namespace ns3
{
TcpContext::TcpContext(int tcpId)//, uint32_t timer_duration)
    : m_Una{0},
      m_Nxt{0},
      m_Wnd{4096},
      m_Iss{0},
      m_Smss{1024},
      m_dupSeq{0},
      m_dupAmount{0},
      m_cnwd{4096}, // RFC5681, IW = 4 * SMSS bytes
      m_ssthresh{1048576}, // 2^20
      //m_time_out{MtTimer(timer_duration, tcpId)},
      m_dupThreshold{3},
      m_congPhase{MtCongPhase::NONE}
{
}

MTState::MTState(ModularTransport* mt)
    : m_library{std::map<int, TcpContext>()},
      m_mt{mt}
{
}

TcpContext
MTState::GetVal(int id)
{
    if (this->m_library.find(id) == this->m_library.end())
    {
        return TcpContext();
    }
    return this->m_library[id];
}

void
MTState::Write(int id, TcpContext data)
{
    this->m_library[id] = data;
}


} // namespace ns3
