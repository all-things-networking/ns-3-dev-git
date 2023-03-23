#include "TCP-timer.h"
#include "TCP-scheduler.h"
#include "TCP-event.h"
namespace ns3{
    Timer::Timer (int duration, Scheduler*){
        this->duration = duration;
        this->Scheduler = scheduler;
    }

    void Timer::start(){
        double now = Simulator::Now().GetSeconds();
        Time expiration_time = Time(now + duration);
        EventID event_id = Simulator::Schedule(expiration_time, &expire, this);
    };

    void Timer::reset(){
	    Simulator::Cancel(event_id);
	    double now = Simulator::Now().GetSeconds();
        Time expiration_time = Time(now + duration);
        this->event_id = Simulator::Schedule(expiration_time, &expire, this);
    }

    void Timer::reset(int duration){
        Simulator::Cancel(event_id);
        this->duration = duration;
        double now = Simulator::Now().GetSeconds();
        Time expiration_time = Time(now + duration);
        this->event_id = Simulator::Schedule(expiration_time, &expire, this);
    }

     void Timer::expire(){
          //create TimerExpired event and add to scheduler

          this->scheduler
     }

private:
     int duration;
     EventID event_id;
     scheduler;
}
}
