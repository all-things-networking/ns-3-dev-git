#ifndef TCP_CONTEXT_H
#define TCP_CONTEXT_H
#include "TCP-scheduler.h"
namespace ns3{
class Timer {
public:
     Timer (int duration, TCPScheduler*);
     void start();
     void reset();
     void reset(int duration);
     void expire();

    private:
        int duration;
        EventID event_id;
        TCPScheduler scheduler;
    }
}
#endif
