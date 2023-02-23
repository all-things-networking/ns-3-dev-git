#ifndef TCP_DISPATCHER_H
#define TCP_DISPATCHER_H
#include "mt-dispatcher.h"

namespace ns3
{
class MTEvent;
class MTEventQueue;
class MTHeader;

class QUICDispatcher : public MTDispatcher
{
  public:
    QUICDispatcher();
    MTEventProcessor* dispatch(MTEvent);
};
} // namespace ns3
#endif
