#ifndef TCP_TIMER_H
#define TCP_TIMER_H
#include "TCP-scheduler.h"
#include "ns3/event-id.h"
namespace ns3{
class ModularTransport;
class TCPTimer {
public:
     TCPTimer();
     TCPTimer (float duration, MTScheduler*);
     void start();
     void reset();
     void reset(float duration);
     float duration;
     EventId event_id;
     MTScheduler* scheduler;
     void expire();
    int notstarted = 1;

};
}
#endif
