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
    TCPTimer::TCPTimer (float duration, MTScheduler* scheduler, ModularTransport* mt){
        this->duration = duration;
        this->scheduler = scheduler;
        this->mt = mt;
    }

    void TCPTimer::start(){
        if (this->notstarted == 1){
            this->event_id = Simulator::Schedule(Seconds(this->duration), &TCPTimer::expire, this);
            this->notstarted = 0;
        }
    };

    void TCPTimer::reset(){
	    Simulator::Cancel(this->event_id);
        this->event_id = Simulator::Schedule(Seconds(this->duration), &TCPTimer::expire, this);
    }

    void TCPTimer::reset(float duration){
        Simulator::Cancel(this->event_id);
        this->duration = duration;
        this->event_id = Simulator::Schedule(Seconds(this->duration), &TCPTimer::expire, this);
    }

 void TCPTimer::expire(){
    std::cout<<"Timer Expire Added to Queue"<<std::endl;
    TimerExpire* newEvent = new TimerExpire(1); //store flow_id
    this->scheduler->AddEvent(newEvent, mt);
           //create TimerExpired event and add to schedule
 }
}
