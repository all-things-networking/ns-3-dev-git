#ifndef QUIC_CONTEXT_H
#define QUIC_CONTEXT_H

#include <ctime> // std::time_t
#include <map>
#include "ns3/ipv4-address.h"
#include "../mt-context.h"
#include "QUIC-StreamContext.h"
namespace ns3
{
class QUICStreamContext;

class MTContext;
/**
 * \brief The class for maintaing the state variables for ONE QUIC connection.
 */
// QUIC context
class QUICContext: public MTContext
{
public:
    QUICContext(int id);
    ~QUICContext();

private:
    std::map<int, QUICStreamContext*> quic_streams; // streams for this connection
};

} // namespace ns3

#endif
