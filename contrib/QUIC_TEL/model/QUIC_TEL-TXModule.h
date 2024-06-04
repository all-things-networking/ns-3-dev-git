#ifndef _QUIC_TX_MODULE_
#define _QUIC_TX_MODULE_
#include "ns3/mt-txmodule.h"
#include "QUIC_TEL.h"
#include "ns3/packet.h"

class QUICTXModule : MTTXModule {
    QUIC_TEL *quic;
    public:
    QUICTXModule(QUIC_TEL *quic);

    void send(Packet *pkt) override;
}

#endif