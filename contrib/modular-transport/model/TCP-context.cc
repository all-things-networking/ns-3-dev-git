#include "mt-state.h"
#include "mt-context.h"
#include "TCP-context.h"
#include "TCP-scheduler.h"
#include <ctime>
#include <map>

namespace ns3
{
TCPContext::TCPContext(int tcpId, TCPScheduler* scheduler)//, uint32_t timer_duration)
    : data{NULL},
      m_Una{0},
      m_Nxt{0},
      m_start{0},
      m_Wnd{4},
      m_Iss{0},
      m_Smss{1024},
      m_dupSeq{0},
      m_dupAmount{0},
      m_cnwd{4096}, // RFC5681, IW = 4 * SMSS bytes
      m_ssthresh{1048576}, // 2^20
      m_segmentsize{4},
      m_dupThreshold{3},
      RTOTimer{NULL}
      //TODO: duration=1, scheduler
{
}
TCPContext::~TCPContext(){}
void TCPContext::SetTimer(int duration, TCPScheduler* scheduler){
    this->RTOTimer.duration = duration;
    this->RTOTimer.scheduler = scheduler;
}
} // namespace ns3
