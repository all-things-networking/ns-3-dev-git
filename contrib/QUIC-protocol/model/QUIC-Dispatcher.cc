#include "QUIC-Dispatcher.h"
#include "QUIC-Event.h"
#include "ns3/mt-event.h"
#include "ns3/mt-eventprocessor.h"


#include "../helper/QUIC-EventProcessor.h"
#include "../helper/QUIC-StreamHandler.h"
#include "../helper/QUIC-LossDetection.h"
#include "../helper/QUIC-PacketDemultiplexer.h"
#include "../helper/QUIC-BufferManagement.h"
#include "../helper/QUIC-ReceiverAckHandler.h"

#include <iostream>

namespace ns3{
QUICDispatcher::QUICDispatcher(){}

std::vector<MTEventProcessor*> QUICDispatcher::dispatch(MTEvent* event){

    //////////////////////////// Sender ////////////////////////////
    std::vector<MTEventProcessor*> ChosenProcessors;
    if (event->type == EventType::STREAM_EVENT) {
        MTEventProcessor* ChosenProcessor = new QUICStreamHandler();
        ChosenProcessors.push_back(ChosenProcessor);
        return ChosenProcessors;
    }

    if (event->type == EventType::RESPONSE_EVENT) {
        MTEventProcessor* ChosenProcessor = new QUICLossDetection();
        ChosenProcessors.push_back(ChosenProcessor);
        return ChosenProcessors;
    }
    ////////////////////////////////////////////////////////////////

    //////////////////////////// Receiver //////////////////////////
    // for now, just returning the 3 processors to process a receivepkt event
    if (event->type == EventType::RECEIVEPKT_EVENT) {
        ChosenProcessors.push_back(new QUICPacketDemultiplexer());
        ChosenProcessors.push_back(new QUICBufferManagement());
        ChosenProcessors.push_back(new QUICAckHandler());
        std::cout << " --------- ----- RECEIVEPKT_EVENT REACHED -------" << std::endl;
        return ChosenProcessors;
    }
    ////////////////////////////////////////////////////////////////


    return ChosenProcessors;
}
}