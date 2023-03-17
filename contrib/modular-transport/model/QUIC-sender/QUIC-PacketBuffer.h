#ifndef QUIC_PACKET_BUFFER_H
#define QUIC_PACKET_BUFFER_H

#include <ctime> // std::time_t
#include <map>
#include <deque>
#include "QUIC-Frame.h"
namespace ns3
{

class QUICFrame;
class Packet;

/**
 * \brief The class for a QUIC PacketBuffer
 */
// QUIC PacketBuffer
class QUICPacketBuffer
{
public:
    QUICPacketBuffer();
    ~QUICPacketBuffer();

    void AddFrame(QUICFrame* frame);
    Ptr<Packet> CreatePacket();
    bool IsFull();
    bool CanAddFrame(QUICFrame* frame);

    // Need to change tehse values
    int CurrentBufferSize = 0;
    int MaxBufferSize = 1000; // Need to change this
    int MaxPacketSize = 20; 
    std::deque<QUICFrame*> frames;

    // This is in case we need to retransmit packets
    std::deque<Ptr<Packet>> sent;

};

} // namespace ns3

#endif
