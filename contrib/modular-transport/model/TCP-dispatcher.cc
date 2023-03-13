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
    else{
    std::cout<<"undefined event"<<std::endl;
    MTEventProcessor* AckProcessor = new AckHandler();
            return AckProcessor;
    }
}
}
