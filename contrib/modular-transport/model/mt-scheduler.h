#ifndef MT_SCHEDULER
#define MT_SCHEDULER

#include <nstime.h>

#include <queue>

namespace ns3
{

class MtEvent;

/**
 * \brief Scheduler for events that choose which event to process next.
 * 
 * Only one instance should exits.
 */
class MtScheduler
{
public:
    MtScheduler();

    /**
     * \brief Return the next event to process
     * \return The event to be processed next.
     * 
     * TODO: Handle when the queue is empty. Pointer
     */
    MtEvent NextEvent();

    /**
     * \brief Put event into the queue for process in the future.
     * \param e Event to push in.
     */
    void PushInEvent(MtEvent e);

private:
    std::queue<MtEvent> m_timeQ;    // All events of time triggered
    std::queue<MtEvent> m_receiveQ; // All events of received packets
    std::queue<MtEvent> m_eventQ;   // All events other than time triggered or received packets
};


}

#endif