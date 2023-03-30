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
    TCPTimer::TCPTimer (int duration, MTScheduler* scheduler){
        this->duration = duration;
        this->scheduler = scheduler;
    }

    void TCPTimer::start(){
        if (this->notstarted == 1){
            this->event_id = Simulator::Schedule(Seconds(this->duration), &TCPTimer::expire, this);
            this->notstarted = 0;
        }
    };

    void TCPTimer::reset(){
	    Simulator::Cancel(event_id);
        this->event_id = Simulator::Schedule(Seconds(this->duration), &TCPTimer::expire, this);
    }

    void TCPTimer::reset(int duration){
        Simulator::Cancel(this->event_id);
        this->duration = duration;
        this->event_id = Simulator::Schedule(Seconds(this->duration), &TCPTimer::expire, this);
    }

 void TCPTimer::expire(){
    //TODO: do we need any locks, ore simlulator scheduler solves that for us?
    TimerExpire* newEvent = new TimerExpire(1); //store flow_id
    this->scheduler->AddEvent(newEvent);
           //create TimerExpired event and add to schedule
 }
}
