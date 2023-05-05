#include "ns3/mt-header.h"
#include "QUICReceiver-Header.h"
namespace ns3{

MTQUICShortHeader::MTQUICShortHeader(){

    destConnID[0] = { 0 };
}
MTQUICShortHeader::~MTQUICShortHeader(){

}

TypeId
MTQUICShortHeader::GetTypeId()
{
    static TypeId tid = TypeId("ns3::MTQUICShortHeader")
                            .SetParent<Header>()
                            .SetGroupName("ModularTransport")
                            .AddConstructor<MTQUICShortHeader>();
    return tid;
}

TypeId
MTQUICShortHeader::GetInstanceTypeId() const
{
    return GetTypeId();
}
void
MTQUICShortHeader::Print(std::ostream& os) const
{
    os << "(" << pckNum << ")";
}

uint32_t
MTQUICShortHeader::GetSerializedSize() const
{
    return 25;
}
void
MTQUICShortHeader::Serialize(Buffer::Iterator start) const
{   
    Buffer::Iterator i = start;
    uint8_t firstByte = headerForm * 128 + fixedBit * 64 + spinBit * 32 + reservedBits * 8 + keyPhase * 4 + pckNumLength;
    i.WriteU8(firstByte);
    i.Write(destConnID, 20);
    i.WriteHtonU32 (pckNum);
}
uint32_t
MTQUICShortHeader::Deserialize(Buffer::Iterator start)
{   
    uint32_t skip = MTHeader::GetSerializedSize();
    start.Next(skip);

    Buffer::Iterator i = start;
    uint8_t firstByte = i.ReadU8();
    headerForm = firstByte / 128;
    fixedBit = (firstByte % 128) / 64;
    spinBit = (firstByte % 64) / 32;
    reservedBits = (firstByte % 16) / 8;
    keyPhase = (firstByte % 8) / 4;
    pckNumLength = (firstByte % 4);
    i.Read(destConnID, 20);
    pckNum = i.ReadNtohU32 ();
    return GetSerializedSize();
}

}