#ifndef QUIC_DISPATCHER_H
#define QUIC_DISPATCHER_H
#include "mt-dispatcher.h"
namespace ns3
{
class MTEvent;
class MTEventProcessor;
class MTDispatcher;
class QUICDispatcher:public MTDispatcher{
    public:
        QUICDispatcher();
        MTEventProcessor* dispatch(MTEvent);
};
}
#endif
