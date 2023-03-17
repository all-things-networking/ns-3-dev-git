#include "QUIC-PacketBuffer.h"
#include "QUIC-Frame.h"

#include <ctime>
#include <map>

namespace ns3
{

QUICPacketBuffer::QUICPacketBuffer()
{
}

QUICPacketBuffer::~QUICPacketBuffer()
{
}

void QUICPacketBuffer::AddFrame(QUICFrame* frame)
{
    if (CanAddFrame(frame)) 
    {
        CurrentBufferSize += frame->GetSize();
        frames.emplace_back(frame);
    }
    
}

Ptr<Packet> QUICPacketBuffer::CreatePacket()
{
    
    Ptr<Packet> res = Create<Packet>(MaxPacketSize);

    int resSize = 0;
    while (resSize < MaxPacketSize && frames.size() > 0) {
        QUICFrame* currFrame = frames.front();
        frames.pop_front();

        // To simulate adding frames together, we use the "AddAtEnd" to append to res packet
        res->AddAtEnd(currFrame->data);

        // TODO: handle headers and sequence numbers n stuff
    }

    return res;
}

bool QUICPacketBuffer::CanAddFrame(QUICFrame* frame)
{
    return CurrentBufferSize + frame->GetSize() <= MaxBufferSize;
}

bool QUICPacketBuffer::IsFull()
{
    return CurrentBufferSize > MaxBufferSize;
}


} // namespace ns3
