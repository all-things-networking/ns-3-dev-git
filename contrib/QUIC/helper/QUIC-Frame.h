#ifndef QUIC_FRAME_H
#define QUIC_FRAME_H
#include <ctime> // std::time_t
#include <map>
#include "ns3/ipv4-l3-protocol.h"

namespace ns3
{

class Packet;

enum class FrameState {
  SENT,
  ACKED,
  NOT_SENT
};

enum FrameType {
    ACK,
    RESET_STREAM,
    STREAM
};

class FrameFields {
public:
  virtual ~FrameFields() {}
};

/*
[RFC 9000]
[https://www.rfc-editor.org/rfc/rfc9000.html#name-ack-frames]

ACK Frame {
  Type (i) = 0x02..0x03,
  Largest Acknowledged (i),
  ACK Delay (i),
  ACK Range Count (i),
  First ACK Range (i),
  ACK Range (..) ...,
  [ECN Counts (..)],
}
*/
class ACKFrameFields : public FrameFields {
  public:
    int LargestACKed;
};

/*
[RFC 9000]
[https://www.rfc-editor.org/rfc/rfc9000.html#name-stream-frames]

STREAM Frame {
  Type (i) = 0x08..0x0f,
  Stream ID (i),
  [Offset (i)],
  [Length (i)],
  Stream Data (..),
}
*/
class StreamFrameFields : public FrameFields {
  public:
    int Size;
    int StreamID;
    int Offset;
    int Length;
    std::string data; // TODO: modify this to general data type
    bool Fin;
};


/*
[RFC 9000]
[https://www.rfc-editor.org/rfc/rfc9000.html#name-frames-and-frame-types]

Frame {
  Frame Type (i),
  Type-Dependent Fields (..),
}
*/
class QUICFrame
{
public:
    QUICFrame(FrameType type, FrameFields * fields);
    QUICFrame(std::string frame);
    QUICFrame();
    ~QUICFrame();

    Ptr<Packet> data; // TODO: This will be replaced with "fields" below

    FrameState state;
    FrameType type;
    FrameFields * fields;

    uint32_t GetSize();
    std::string GetData();
    std::string generateHeader();

    /**
     * Turn the frame into a byte buffer
    */
    void Serialize(uint8_t* buffer);
    void Deserialize();
};

} // namespace ns3

#endif
