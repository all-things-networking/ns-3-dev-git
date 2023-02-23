#include "mt-state.h"
#include "mt-context.h"
#include "TCP-context.h"
#include <ctime>
#include <map>

namespace ns3
{
TcpContext::TcpContext(int tcpId)//, uint32_t timer_duration)
    : data{NULL},
      m_Una{0},
      m_Nxt{0},
      m_Wnd{4096},
      m_Iss{0},
      m_Smss{1024},
      m_dupSeq{0},
      m_dupAmount{0},
      m_cnwd{4096}, // RFC5681, IW = 4 * SMSS bytes
      m_ssthresh{1048576}, // 2^20
      //m_time_out{MtTimer(timer_duration, tcpId)},
      m_dupThreshold{3}
{
}
TcpContext::~TcpContext(){}
} // namespace ns3
