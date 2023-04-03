#include "mt-scheduler.h"
#include "ns3/core-module.h"
#include "modular-transport.h"
namespace ns3{
    void MTScheduler::SetModularTransport(ModularTransport* mt){
        this->mt = mt;
    }
    void MTScheduler::AddEvent(MTEvent* newEvent){
        this->enqueue(newEvent);
        if(!this->mt->IsActive()){
            Simulator::ScheduleNow(&ModularTransport::Mainloop, this->mt);
        }
    }
}
