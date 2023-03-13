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
    if (anything->Type == TCPEventType::Send_Data){
        std::cout<<"dispatched SendEvent"<<std::endl;
        MTEventProcessor* SendProcessor = new SendIfPossible();
        return SendProcessor;
    }
    else if(anything->Type == TCPEventType::Ack_Data){
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
