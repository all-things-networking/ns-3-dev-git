#include "mt-event.h"

namespace ns3
{
MtEvent::MtEvent(int id, MtEvent::Type type)
    : m_id{id},
      m_type{type},
      m_creationTime{std::time(NULL)}
{
}

MtAckEvent::MtAckEvent(int id, uint32_t ackNum)
    : MtEvent(id, MtEvent::Type::RECEIVE),
      m_ackNum{ackNum}
{
}

MtSendEvent::MtSendEvent(int id)
    : MtEvent(id, MtEvent::Type::OTHER)
{
}

MtCongNotifyEvent::MtCongNotifyEvent(int id, uint32_t lastAckNum)
    : MtEvent(id, MtEvent::Type::OTHER),
      m_lastAckNum{lastAckNum}
{
}

TcpTimeoutEvent::TcpTimeoutEvent(int id)
    : MtEvent(id, MtEvent::Type::TIMEOUT)
{
}

} // namespace ns3