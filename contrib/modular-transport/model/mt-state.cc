#include "mt-state.h"
#include "mt-context.h"
#include <ctime>
#include <map>

namespace ns3
{
MTState::MTState(){}
MTState::MTState(ModularTransport* mt)
    : m_library{std::map<int, MTContext*>()},
      m_mt{mt}
{
}

MTContext*
MTState::GetVal(int id)
{
    if (this->m_library.find(id) == this->m_library.end())
    {
        auto newContext = new MTContext();
        return newContext;
    }
    return this->m_library[id];
}

void
MTState::Write(int id, MTContext* data)
{
    this->m_library[id] = data;
}


} // namespace ns3
