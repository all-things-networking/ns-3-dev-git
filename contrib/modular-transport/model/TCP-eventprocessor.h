#ifndef TCP_EVENTPROCESSOR_H
#define TCP_EVENTPROCESSOR_H
#include "mt-eventprocessor.h"
#include <vector>
#include <utility> // std::pair

//mostly copied from eric's branch, though send if possible is mine
namespace ns3
{

class MTEvent;
class MTContext;
/**
 * \brief The base class for event processor. This is virtual class.
 */
struct EventProcessorOutput;
class MTEventProcessor;
class SendIfPossible: public MTEventProcessor
{
public:
    SendIfPossible();
    EventProcessorOutput* Process(MTEvent* e, MTContext* c);
    bool IsValidEvent(MTEvent e);
};

class AckHandler: public MTEventProcessor{
    public:
    AckHandler();
    EventProcessorOutput* Process(MTEvent* e, MTContext* c);
    bool IsValidEvent(MTEvent e);
};

class TimedResendHandler: public MTEventProcessor{
    public:
    TimedResendHandler();
    EventProcessorOutput* Process(MTEvent* e, MTContext* c);
    bool IsValidEvent(MTEvent e);
};// namespace ns3
}
#endif
