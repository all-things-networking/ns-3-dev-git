#ifndef QUICRECEIVER_HEADER_H
#define QUICRECEIVER_HEADER_H
#include <vector>
#include "ns3/mt-header.h"
#include "QUICReceiver-Frame.h"
namespace ns3{


// Modular transport QUIC long header
// RTC-9000 17.2
class MTQUICLongHeader : public MTHeader {
    public:

        uint8_t headerForm; // first bit, set to 1 for long header
        uint8_t fixedBit = 1; // set to 1 unless it's version negotiation 
        uint8_t destConnID[20]; //0-160 bits 

        uint8_t longPckType; //2 bits
        uint8_t pckTypeBits; //4 bits
        uint32_t version; // 32 bits
        uint8_t destConnIDLen; //8 bits
        uint8_t srcConnIDLen; //8 bits
        uint8_t srcConnID[20]; //0-160 bits 

        uint8_t pckNumlen; // 2 bits
        uint64_t length; // length of the remainder of the packet
        uint8_t pckNum; // 1-4 bits

        static TypeId GetTypeId (void);
        virtual TypeId GetInstanceTypeId() const override;
        virtual void Print(std::ostream& os) const override;
        virtual uint32_t GetSerializedSize() const override;
        virtual void Serialize(Buffer::Iterator start) const override;
        virtual uint32_t Deserialize(Buffer::Iterator start) override;

};

class InitialPacketHeader: public MTQUICLongHeader {

};

class ZeroRTTHeader: public MTQUICLongHeader {

};

// QUIC short header
// RFC-9000 17.3
/*
1-RTT Packet {
  Header Form (1) = 0,
  Fixed Bit (1) = 1,
  Spin Bit (1),
  Reserved Bits (2),
  Key Phase (1),
  Packet Number Length (2),
  Destination Connection ID (0..160),
  Packet Number (8..32),
  Packet Payload (8..),
}
*/
class MTQUICShortHeader : public MTHeader {

    public:
        uint8_t headerForm = 0; // first bit, set to 0 for short header
        uint8_t fixedBit = 1; // set to 1 unless it's version negotiation 
        uint8_t spinBit; 
        uint8_t reservedBits; 
        uint8_t keyPhase; 
        uint8_t pckNumLength; 
        uint8_t destConnID[20]; //0-160 bits 
        uint32_t pckNum;

        MTQUICShortHeader();
        ~MTQUICShortHeader();

        static TypeId GetTypeId (void);
        virtual TypeId GetInstanceTypeId() const override;
        virtual void Print(std::ostream& os) const override;
        virtual uint32_t GetSerializedSize() const override;
        virtual void Serialize(Buffer::Iterator start) const override;
        virtual uint32_t Deserialize(Buffer::Iterator start) override;
};
}
#endif