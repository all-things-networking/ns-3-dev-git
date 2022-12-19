#include "mt-state.h"

#include <ctime>
#include <map>

namespace ns3
{
TcpContext::TcpContext()
    : m_Una{0},
      m_Nxt{0},
      m_Wnd{4096}, 
      m_Iss{0},
      m_Smss{1024},
      m_dupSeq{0},
      m_dupAmount{0},
      m_cnwd{4096}, // RFC5681, IW = 4 * SMSS bytes
      m_ssthresh{1048576}, // 2^20
      m_time_out{std::time(NULL)},
      m_dupThreshold{3},
      m_congPhase{MtCongPhase::NONE}
{
}

MtState::MtState()
    : m_library{std::map<int, TcpContext>()}
{
}

TcpContext
MtState::GetVal(int id)
{
    if (this->m_library.find(id) == this->m_library.end())
    {
        return TcpContext();
    }
    return this->m_library[id];
}

void
MtState::Write(int id, TcpContext data)
{
    this->m_library[id] = data;
}


} // namespace ns3