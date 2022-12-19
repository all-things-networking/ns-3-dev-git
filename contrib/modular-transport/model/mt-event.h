#ifndef MT_EVENT_H
#define MT_EVENT_H

#include <string>
#include <ctime>

namespace ns3
{

/**
 * \brief The base class for Event instance that passing between components.
 */
class MtEvent
{
public:
    enum Type
    {
        TIME,
        RECEIVE,
        TIMEOUT,
        OTHER
    };

    MtEvent(int id, MtEvent::Type type);
    virtual ~MtEvent();

    int m_id;                // Mt connection id, custom defined
    MtEvent::Type m_type;    // Type of the Event
    time_t m_creationTime;   // creation time of Event instance
};


/**
 * \brief Event for one acknowledgement packet is received when instance created.
 */
class MtAckEvent: public MtEvent
{
public:
    MtAckEvent(int id, uint32_t ackNum);

    uint32_t m_ackNum;  // ACK number
};

/**
 * \brief Event for sending one packet
 * TODO: Complete Implementation
 * 
 * The packet should sent right away without checking if isFastRetrans == true
 */
class MtSendEvent: public MtEvent
{
public:
    MtSendEvent(int id);
};

/**
 * \brief Event for sending one packet
 * TODO: Complete Implementation
 * 
 * The packet should sent right away without checking if isFastRetrans == true
 */
class MtCongNotifyEvent: public MtEvent
{
public:
    MtCongNotifyEvent(int id, uint32_t lastAckNum);

    uint32_t m_lastAckNum; // latest received ack number
};

/**
 * \brief Event for timeouts
 * TODO: Complete Implementation
 * 
 * The packet should sent right away without checking if isFastRetrans == true
 */
class TcpTimeoutEvent: public MtEvent
{
public:
    TcpTimeoutEvent(int id);
};



} // namespace ns3

#endif