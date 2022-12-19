#include "mt-scheduler.h"

#include "mt-event.h"

namespace ns3
{
MtScheduler::MtScheduler()
    : m_timeQ{std::queue<MtEvent>()},
      m_receiveQ{std::queue<MtEvent>()},
      m_eventQ{std::queue<MtEvent>()}
{
}

MtEvent
MtScheduler::NextEvent()
{
    // TODO: Imcomplete
    return m_timeQ.pop();
}

void
MtScheduler::PushInEvent(MtEvent e)
{
    switch (e.m_type)
    {
        case MtEvent::Type::TIME:
            m_timeQ.push(e);
            break;
        case MtEvent::Type::RECEIVE:
            m_receiveQ.push(e);
            break;
        default:
            // MtEvent::Type::OTHER
            m_eventQ.push(e);
    }
}

}