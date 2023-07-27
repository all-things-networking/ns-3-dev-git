#ifndef QUIC_FRAME_H
#define QUIC_FRAME_H
#include <ctime> // std::time_t
#include <map>
#include <utility>   // std::pair
#include <vector>
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/mt-header.h"

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
    STREAM,
    MAX_DATA, // max data can be sent on the connection as a whole
    MAX_STREAM_DATA, // max data can be sent on a stream
    MAX_STREAM, // max cumulative number of streams on a given type it is permitted to open
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
// Not being used anymore
class StreamFrameFields : public FrameFields {
  public:
    int Size; // don't know what this is yet
    int StreamID;
    int Offset;                 // offset from stream beginning
    int Length;                 // current frame's size
    std::string data;           // TODO: modify this to general data type
    bool Fin;
};

// somewhere to store the frame type

class QUICFrameHeader : public MTHeader {
  void SerializeStreamFrame(Buffer::Iterator i) const;
  void SerializeACKFrame(Buffer::Iterator i) const;

  void DeserializeStreamFrame(Buffer::Iterator i);
  void DeserializeACKFrame(Buffer::Iterator i);

  public:
    uint32_t offset; // TODO: figure out what it should be in RFC9000
    uint32_t streamID;
    uint32_t length; // size of data, right now only STREAM frame use this
    uint32_t frameType;
    // fin bit
    bool fin;

    // if frameType is ACK, the following field are used
    uint32_t largestACKed;
    uint32_t ackRangeCount;
    uint32_t firstACKRange;
    std::vector<std::pair<uint32_t, uint32_t>> ACKRanges; // gap, ack range

    

    // the currentFrameSize can vary depending on which FrameType it is
    // currently, for ACK -> something
    // others -> 20
    int currentFrameSize;


    QUICFrameHeader(uint32_t streamID, uint32_t offset, uint32_t length, FrameType frameType, bool finBit,
      uint32_t largestACKed, uint32_t ackRangeCount, uint32_t firstACKRange);
    QUICFrameHeader();

    void setACKFrameHeader(uint32_t largestACKed, uint32_t ackRangeCount, uint32_t firstACKRange);
    void setStreamFrameHeader(uint32_t streamID, uint32_t offset, uint32_t length, bool finBit);
    void setMaxStreamFrameHeader();
    void setMaxDataFrameHeader();

    void ACKRangesPush(uint32_t gap, uint32_t length);

    TypeId GetInstanceTypeId() const override;
    void Print(std::ostream& os) const override;
    uint32_t GetSerializedSize() const override;
    void Serialize(Buffer::Iterator start) const override;
    uint32_t Deserialize(Buffer::Iterator start) override;
};

/*
[RFC 9000]
[https://www.rfc-editor.org/rfc/rfc9000.html#name-frames-and-frame-types]

Frame {
  Frame Type (i),
  Type-Dependent Fields (..),
}
*/
class QUICFrame : public Packet
{
  // az comment: new QUICFrame structure, it can be just a packet
  // Probably it needs to have some kind of type -> e.g. Data to be Send or ACK Frame
  /*
    RFC9000
    Some Frame Types
    MAX_STREAM_DATA:
      - Used by receiver to advertise a larger limit for a stream
      - needs a stream ID
      - indicates the maximum absolute byte offset of a stream

  */
public:
    QUICFrame(FrameType type, FrameFields * fields);
    QUICFrame(std::string frame);
    
    QUICFrame();
    ~QUICFrame();

    QUICFrameHeader header;
    Ptr<Packet> data; // TODO: This will be replaced with "fields" below
    // NS3 Packet has AddHeader Function

    FrameState state;
    FrameType type;
    FrameFields * fields;

    uint32_t GetSize();
    std::string GetData();
    std::string generateHeader();

    /**
     * Turn the frame into a byte buffer
    */
    // currently the following two are not used
    void Serialize(uint8_t* buffer);
    void Deserialize();
    
    // void AddHeader(QUICFrameHeader &)
};

} // namespace ns3

#endif
