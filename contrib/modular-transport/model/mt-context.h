#ifndef MT_CONTEXT_H
#define MT_CONTEXT_H

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


} // namespace ns3

#endif
