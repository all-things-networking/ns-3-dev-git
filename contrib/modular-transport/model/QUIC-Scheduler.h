#ifndef MT_QUIC_SCHEDULER_H
#define MT_QUIC_SCHEDULER_H
#include "mt-scheduler.h"
#include "QUIC-Event.h"
#include <queue>
#include <vector>
namespace ns3{
class MTScheduler;
class MTEvent;
class MTHeader;
class Packet;
class QUICScheduler: public MTScheduler{
    public:
    std::queue<MTEvent*> myqueue;
    QUICScheduler();
    MTEvent* GetNextEvent();
    void AddEvent(MTEvent*);
    MTEvent* CreateSendEvent(int, long);
    MTEvent* SendPacket(int flow_id, long time);
    std::vector<MTEvent*> SendString(int flow_id, long time, std::string text, int stream);
    bool isEmpty();
    };
}

#endif
