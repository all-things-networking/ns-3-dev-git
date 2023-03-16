#ifndef MT_QUIC_DISPATCHER_H
#define MT_QUIC_DISPATCHER_H
#include "mt-dispatcher.h"

namespace ns3
{

class QUICDispatcher:public MTDispatcher{
    public:
    QUICDispatcher();
    std::vector<MTEventProcessor*> dispatch(MTEvent*);
};

}
#endif
