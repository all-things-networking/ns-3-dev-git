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
#include "../helper/QUIC-SendPacket.h"
#include "../helper/QUIC-AddStreamData.h"
#include "../helper/QUIC-HandleReceiveACK.h"


#include <iostream>

namespace ns3{
QUICDispatcher::QUICDispatcher(){}

std::vector<MTEventProcessor*> QUICDispatcher::dispatch(MTEvent* event){

    //////////////////////////// Sender ////////////////////////////
    std::vector<MTEventProcessor*> ChosenProcessors;
    QUICEvent* quicEvent = dynamic_cast<QUICEvent*>(event);
    MTEventProcessor* ChosenProcessor;

    if (quicEvent->type == EventType::STREAM_EVENT) {
        StreamEvent* streamEvent = dynamic_cast<StreamEvent*>(event);

        // If we have a SEND_PACKET event we will send the packet
        if (streamEvent->streamEventType == StreamEventType::SEND_PACKET)
        {
            ChosenProcessor = new QUICSendPacket();
        }

        // If we have a ADD_DATA event, create a dataFrame and send it
        if (streamEvent->streamEventType == StreamEventType::ADD_DATA)
        {
            ChosenProcessor = new QUICAddStreamData();
        }

        ChosenProcessors.push_back(ChosenProcessor);
        return ChosenProcessors;
    }

    if (quicEvent->type == EventType::RESPONSE_EVENT) {
        ResponseEvent* responseEvent = dynamic_cast<ResponseEvent*>(event);
        if (responseEvent->responseEventType == ResponseEventType::ACK_PACKET)
        {
            ChosenProcessor = new QUICLossDetection();
        }
        ChosenProcessors.push_back(ChosenProcessor);
        return ChosenProcessors;
    }
    ////////////////////////////////////////////////////////////////

    //////////////////////////// Receiver //////////////////////////
    // for now, just returning the 3 processors to process a receivepkt event
    if (quicEvent->type == EventType::RECEIVEPKT_EVENT) {
        ChosenProcessors.push_back(new QUICPacketDemultiplexer());
        ChosenProcessors.push_back(new QUICBufferManagement());
        ChosenProcessors.push_back(new QUICAckHandler());
        return ChosenProcessors;
    }
    ////////////////////////////////////////////////////////////////


    return ChosenProcessors;
}
}