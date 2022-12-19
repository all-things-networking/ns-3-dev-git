#include "mt-timer.h"

#include <ns3/nstime.h>
#include <ns3/timer.h>

namespace ns3
{
MtTimer::MtTimer(uint32_t duration, int tcpId)
    : m_tcpId{tcpId},
      m_duration{duration},
      m_ns3Timer{Timer()}
{
}

void
MtTimer::SetDuration(uint32_t duration)
{
    this->m_duration = duration;
    this->Reset();
}

uint32_t
MtTimer::GetDuration()
{
    return this->m_duration;
}

void
MtTimer::Start()
{
    if (this->m_ns3Timer.IsSuspended())
    {
        this->m_ns3Timer.Resume();
    }
    else if (!this->m_ns3Timer.IsExpired() && !this->m_ns3Timer.IsRunning())
    {
        uint64_t millDuration = this->m_duration;
        this->m_ns3Timer.SetFunction(&(this->m_mt.TimeOutNotify));
        this->m_ns3Timer.SetArguments(m_tcpId);
        this->m_ns3Timer.Schedule(Time(MilliSeconds(millDuration)));
    }
}

void
MtTimer::Stop()
{
    if (this->m_ns3Timer.IsRunning())
    {
        this->m_ns3Timer.Suspend();
    }
}

void
MtTimer::Reset()
{
    if (this->m_ns3Timer.IsRunning())
    {
        this->m_ns3Timer.Suspend();
        this->m_ns3Timer.Cancel();
    }
    this->m_ns3Timer = Timer();
}

void
MtTimer::IsFinished()
{
    return this->m_ns3Timer.IsExpired();
}

} // ns3 namespace