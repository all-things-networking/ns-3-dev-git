#include "mt-state.h"
#include <ctime>
#include <map>

namespace ns3
{
MTContext::MTContext(){

}
MTState::MTState(){}
MTState::MTState(ModularTransport* mt)
    : m_library{std::map<int, MTContext>()},
      m_mt{mt}
{
}

MTContext
MTState::GetVal(int id)
{
    if (this->m_library.find(id) == this->m_library.end())
    {
        return MTContext();
    }
    return this->m_library[id];
}

void
MTState::Write(int id, MTContext data)
{
    this->m_library[id] = data;
}


} // namespace ns3
