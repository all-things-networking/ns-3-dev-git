#ifndef QUIC_CONGESTION_CONTROLLER_H
#define QUIC_CONGESTION_CONTROLLER_H
#include "QUIC-Dispatcher.h"

namespace ns3
{
class MTEvent;
class MTEventQueue;
class MTHeader;

class QUICCongestionController
{
  public:
    QUICCongestionController();
};
} // namespace ns3
#endif
