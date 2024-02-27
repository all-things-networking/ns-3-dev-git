#include "QUIC-State.h"
#include "ns3/mt-context.h"
#include <ctime>
#include <map>

// the MTContext is kept unchange

namespace ns3
{
QUICState::QUICState(){}
QUICState::QUICState(QUIC_TEL* mt)
    : m_library{std::map<int, MTContext*>()},
      m_mt{mt}
{
}

MTContext*
QUICState::GetVal(int id)
{
    if (this->m_library.find(id) == this->m_library.end())
    {
        auto newContext = new MTContext();
        return newContext;
    }
    return this->m_library[id];
}

void
QUICState::Write(int id, MTContext* data)
{
    this->m_library[id] = data;
}


} // namespace ns3
