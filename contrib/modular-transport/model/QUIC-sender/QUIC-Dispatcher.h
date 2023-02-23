#ifndef TCP_DISPATCHER_H
#define TCP_DISPATCHER_H
#include "mt-dispatcher.h"
#include <vector>

namespace ns3
{
class MTEvent;
class MTEventQueue;
class MTHeader;

class QUICDispatcher : public MTDispatcher
{
  private:
    std::vector<MTEventProcessor*> m_processors;

  public:
    QUICDispatcher();
    MTEventProcessor* dispatch(MTEvent);
    void AddProcessor(MTEventProcessor* processor); 
};
} // namespace ns3
#endif
