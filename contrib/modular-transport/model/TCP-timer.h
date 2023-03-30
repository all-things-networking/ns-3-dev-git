#ifndef TCP_TIMER_H
#define TCP_TIMER_H
#include "TCP-scheduler.h"
#include "ns3/event-id.h"
namespace ns3{
class TCPTimer {
public:
     TCPTimer();
     TCPTimer (int duration, MTScheduler*);
     void start();
     void reset();
     void reset(int duration);
     int duration;
     EventId event_id;
     MTScheduler* scheduler;
     void expire();
    int notstarted = 1;

};
}
#endif
