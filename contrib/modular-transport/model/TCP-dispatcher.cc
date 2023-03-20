#include "mt-dispatcher.h"
#include "TCP-dispatcher.h"
#include "mt-event.h"
#include "TCP-event.h"
#include "mt-eventprocessor.h"
#include "TCP-eventprocessor.h"
#include <typeinfo>
namespace ns3{
TCPDispatcher::TCPDispatcher(){}
MTEventProcessor* TCPDispatcher::dispatch(MTEvent* anything){
    TCPEvent* tcpevent = dynamic_cast<TCPEvent*>(anything);
    if (tcpevent == NULL){
        std::cout<<"cast unsuccessful"<<std::endl;
        int a = 0/0; //raise an exception here
    }
    if (tcpevent->Type == TCPEventType::SEND_DATA){
        std::cout<<"dispatched SendEvent"<<std::endl;
        MTEventProcessor* SendProcessor = new SendIfPossible();
        return SendProcessor;
    }
    else if(tcpevent->Type == TCPEventType::ACK_DATA){
        std::cout<<"dispatched AckEvent"<<std::endl;
        MTEventProcessor* AckProcessor = new AckHandler();
        return AckProcessor;
    }
    else if(tcpevent->Type == TCPEventType::TIME_EXPIRE){
            std::cout<<"dispatched TimerEvent"<<std::endl;
            TimedResendHandler* TimerProcessor = new TimedResendHandler();
            return TimerProcessor;
    }
    else{
        std::cout<<"undefined event"<<std::endl;
        MTEventProcessor* AckProcessor = new AckHandler();
        return AckProcessor;
    }
}
}
