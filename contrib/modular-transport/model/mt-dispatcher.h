#ifndef MT_DISPATCHER
#define MT_DISPATCHER

#include <vector>

namespace ns3
{

class MtEventProcessor;
class MtEvent;

/**
 * \brief Dispatcher for events, will choose the correct event processors based 
 * on input events.
 * 
 * Only one instance should exits.
 */
class MtDispatcher
{
public:
    MtDispatcher(std::vector<MtEventProcessor> processors = std::vector<MtEventProcessor>());

    /**
     * \brief Add event processor.
     * \param processor The event processor to be added.
     * 
     * TODO: Check if event processor exits before add.
     */
    void AddProcessor(MtEventProcessor processor);

    /**
     * \brief Return the corresponding processor that can process the event
     * \param e Event to be processed.
     * \return The event processor valid to begin process.
     * 
     * Function will choose the first processor if multiple processors
     * can process the event.
     * Function only choose the process, not process the event.
     */
    MtEventProcessor Dispatch(MtEvent e);

private:
    std::vector<MtEventProcessor> m_processors;
};


}

#endif