#ifndef QUIC_SCHEDULER_H
#define QUIC_SCHEDULER_H
#include "ns3/mt-scheduler.h"
#include "QUIC-Event.h"
#include "../helper/QUIC-PacketBuffer.h"
#include <queue>
#include <vector>
namespace ns3{
class MTScheduler;
class MTEvent;
class MTHeader;
class QUICPacketBuffer;
class QUICScheduler: public MTScheduler{
    public:
    std::queue<MTEvent*> myqueue;
    QUICScheduler();
    MTEvent* GetNextEvent();
    void AddEvent(MTEvent*);
    bool isEmpty();
    };
}

#endif