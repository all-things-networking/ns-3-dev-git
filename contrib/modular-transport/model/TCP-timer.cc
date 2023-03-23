#include "TCP-timer.h"
#include "TCP-scheduler.h"
#include "TCP-event.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/traffic-control-layer.h"
#include "ns3/modular-transport.h"

namespace ns3{
    TCPTimer::TCPTimer(){
        //not usable
    }
    TCPTimer::TCPTimer (int duration, TCPScheduler* scheduler){
        this->duration = duration;
        this->scheduler = scheduler;
    }

    void TCPTimer::start(){
        double now = Simulator::Now().GetSeconds();
        Time expiration_time = Time(now + this->duration);
        this->event_id = Simulator::Schedule(expiration_time, &TCPTimerexpire, this);
    };

    void TCPTimer::reset(){
	    Simulator::Cancel(event_id);
	    double now = Simulator::Now().GetSeconds();
        Time expiration_time = Time(now + this->duration);
        this->event_id = Simulator::Schedule(expiration_time, &TCPTimerexpire, this);
    }

    void TCPTimer::reset(int duration){
        Simulator::Cancel(this->event_id);
        this->duration = duration;
        double now = Simulator::Now().GetSeconds();
        Time expiration_time = Time(now + duration);
        this->event_id = Simulator::Schedule(expiration_time, &TCPTimerexpire, this);
    }

 void TCPTimerexpire(TCPTimer* expiredTimer){
           //create TimerExpired event and add to schedule
 }
}
