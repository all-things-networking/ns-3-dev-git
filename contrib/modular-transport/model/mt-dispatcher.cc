#include "mt-dispatcher.h"

namespace ns3
{
MtDispatcher::MtDispatcher(std::vector<MtEventProcessor> processors)
    : m_processors{processors}
{
}

void
MtDispatcher::AddProcessor(MtEventProcessor processor)
{
    m_processors.emplace_back(processor);
}

MtEventProcessor
MtDispatcher::Dispatch(MtEvent e)
{
    for (auto processor : this->m_processors)
    {
      if (processor.isValidEvent(e))
      {
        return processor;
      }
    }
    // TODO: Use Pointer
}

}