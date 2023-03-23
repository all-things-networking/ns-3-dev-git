#ifndef TCP_TIMER_H
#define TCP_TIMER_H
#include "TCP-scheduler.h"
#include "ns3/event-id.h"
namespace ns3{
class TCPTimer {
public:
     TCPTimer();
     TCPTimer (int duration, TCPScheduler*);
     void start();
     void reset();
     void reset(int duration);
     void expire();

    private:
        int duration;
        EventId event_id;
        TCPScheduler* scheduler;
};
}
#endif
