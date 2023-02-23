#ifndef MT_STATE_H
#define MT_STATE_H

#include <ctime> // std::time_t
#include <map>
#include "ns3/ipv4-address.h"

namespace ns3
{
class ModularTransport;

/**
 * \brief The class indicate phase in congestion control algorithms.
*/
class MTContext{
public:
    ns3::Ipv4Address saddr;
    ns3::Ipv4Address daddr;
    MTContext();
    virtual ~MTContext();

};

/**
 * \brief The class for maintaing the state variables for ALL Mt connections.
 *
 * Currently all contexts are stored on stack.
 * TODO: may move the data storage to heap using alloc or smart pointer.
 */
class MTState
{
public:
    MTState();
    MTState(ModularTransport* mt);

    /**
     * \brief Get the context of Mt connection with id.
     * \param id The id of Mt connection.
     * \return The context that belongs to the specific Mt connection.
     */
    MTContext* GetVal(int id);
    /**
     * \brief Store the context data to m_library, given Mt connection id.
     * \param id The id of Mt connection.
     * \param data The context data of the Mt.
     */
    void Write(int id, MTContext* data);

private:
    ModularTransport* m_mt;
    std::map<int, MTContext*> m_library; // database of all Mt connnections.
};

} // namespace ns3

#endif
