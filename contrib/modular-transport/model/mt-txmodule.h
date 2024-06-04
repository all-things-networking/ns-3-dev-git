#ifndef _MT_TXMODULE_
#define _MT_TXMODULE_
#include "ns3/packet.h"
#include "modular-transport.h"

class MTTXModule {
    MTTXModule(ModularTransport *mt);
    ~MTTXModule();

    virtual void send(Packet *pkt) = 0;
}

#endif