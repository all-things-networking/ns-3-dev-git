#ifndef MT_QUIC_CONTEXT_H
#define MT_QUIC_CONTEXT_H

#include <ctime> // std::time_t
#include <map>
#include <vector>
#include "ns3/ipv4-address.h"
#include "mt-state.h"
#include "QUICStream.h"


namespace ns3
{
class ModularTransport;

// QUIC context
class QUICContext: public MTContext
{ 
public:

    int flow_id;
    uint32_t max_data; // maximum amount of data that can be sent on the connection
    uint32_t max_stream_data; // maximum amount of data that can be sent on a stream
    std::vector<QUICStream> streams;

    QUICContext(int flow_id);

};
} // namespace ns3


#endif