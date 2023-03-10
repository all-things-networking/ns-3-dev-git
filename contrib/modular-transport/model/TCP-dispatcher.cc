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
    if (typeid(anything) == typeid(SendEvent)){
        MTEventProcessor* SendProcessor = new SendIfPossible();
        return SendProcessor;
    }
    else{
        MTEventProcesor* AckProcessor = new AckHandler();
        return AckProcessor;
    }
}
}
