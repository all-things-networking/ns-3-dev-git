#ifndef MT_QUIC_STREAM_H
#define MT_QUIC_STREAM_H
#include <cstdint>

namespace ns3
{

class QUICStream 
{
    
public: 
    enum steamType
    {
        CLIENT_BI,
        CLIENT_UNI,
        SERVER_BI,
        SERVER_UNI
    };

    enum steamState
    {
        
    };

    uint64_t streamId;
    QUICStream::steamType type;
    uint64_t connectionId;
    uint32_t priority;
};
}

#endif