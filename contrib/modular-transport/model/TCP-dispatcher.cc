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
    std::cout<<(typeid(anything) == typeid(anything))<<std::endl;
    std::cout<<(typeid(anything) == typeid(SendEvent*))<<std::endl;
    std::cout<<(typeid(anything) == typeid(AckEvent*))<<std::endl;
    if (typeid(anything) == typeid(SendEvent)){
        std::cout<<"dispatched SendEvent"<<std::endl;
        MTEventProcessor* SendProcessor = new SendIfPossible();
        return SendProcessor;
    }
    else{
        std::cout<<"dispatched AckEvent"<<std::endl;
        MTEventProcessor* AckProcessor = new AckHandler();
        return AckProcessor;
    }
}
}
