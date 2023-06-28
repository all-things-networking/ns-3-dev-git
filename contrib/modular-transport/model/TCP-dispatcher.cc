#include "mt-dispatcher.h"
#include "TCP-dispatcher.h"
#include "mt-event.h"
#include "TCP-event.h"
#include "mt-eventprocessor.h"
#include "TCP-eventprocessor.h"
#include <typeinfo>

namespace ns3 {

TCPDispatcher::TCPDispatcher(){}

std::vector<MTEventProcessor*> TCPDispatcher::dispatch(MTEvent* anything){
    TCPEvent* tcpevent = dynamic_cast<TCPEvent*>(anything);
    std::vector<MTEventProcessor*> chosenProcessors;
    if (tcpevent == NULL){
        std::cout<<"TCPDispatcher::dispatch: invalid event type"<<std::endl;
        //raise an exception here
        throw;
    }
    if (tcpevent->Type == TCPEventType::SEND_DATA){
        //std::cout<<"dispatched SendEvent"<<std::endl;
        MTEventProcessor* SendProcessor = new SendIfPossible();
        chosenProcessors.push_back(SendProcessor);
        return chosenProcessors;
    }
    else if(tcpevent->Type == TCPEventType::ACK_DATA){
        //std::cout<<"dispatched AckEvent"<<std::endl;
        MTEventProcessor* AckProcessor = new AckHandler();
        chosenProcessors.push_back(AckProcessor);
        return chosenProcessors;
    }
    else if(tcpevent->Type == TCPEventType::TIMEOUT){
        //std::cout<<"dispatched TimerEvent"<<std::endl;
        TimeoutHandler* TimerProcessor = new TimeoutHandler();
        chosenProcessors.push_back(TimerProcessor);
        return chosenProcessors;
    }
    else{
        std::cout<<"TCPDispatcher::dispatch: undefined event"<<std::endl;
        throw;
    }

    return chosenProcessors;
}

}
