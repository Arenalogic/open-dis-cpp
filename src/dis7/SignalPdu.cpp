#include <dis7/SignalPdu.h>

#include <stdexcept>

using namespace DIS;

namespace
{
// Data Length (K) is a 16-bit count of BITS, so the data field holds at most
// 65535 bits: 8191 whole octets plus 7 bits.
const std::size_t kMaxDataOctets = 65535 / 8 + 1;

std::size_t octetsForBits(std::size_t bits)
{
    return (bits + 7) / 8;
}

// Zero octets that follow `dataOctets` data octets so the Data field ends on a
// 32-bit boundary.
std::size_t paddingOctets(std::size_t dataOctets)
{
    return (4 - dataOctets % 4) % 4;
}

// Data Length in bits as marshal writes it, without the range check.
std::size_t effectiveDataLength(unsigned short storedBits, std::size_t dataOctets)
{
    return octetsForBits(storedBits) == dataOctets ? storedBits : dataOctets * 8;
}
}


SignalPdu::SignalPdu() : RadioCommunicationsFamilyPdu(),
   _encodingScheme(0),
   _tdlType(0),
   _sampleRate(0),
   _dataLength(0),
   _samples(0),
   _data()
{
    setPduType( 26 );
}

SignalPdu::~SignalPdu()
{
}

unsigned short SignalPdu::getEncodingScheme() const
{
    return _encodingScheme;
}

void SignalPdu::setEncodingScheme(unsigned short pX)
{
    _encodingScheme = pX;
}

unsigned short SignalPdu::getTdlType() const
{
    return _tdlType;
}

void SignalPdu::setTdlType(unsigned short pX)
{
    _tdlType = pX;
}

unsigned int SignalPdu::getSampleRate() const
{
    return _sampleRate;
}

void SignalPdu::setSampleRate(unsigned int pX)
{
    _sampleRate = pX;
}

unsigned short SignalPdu::getDataLength() const
{
    // The stored K stays authoritative while it still describes the data;
    // once the data has been replaced or resized, K is the whole-octet count.
    if(octetsForBits(_dataLength) == _data.size())
    {
        return _dataLength;
    }
    if(_data.size() >= kMaxDataOctets)
    {
        throw std::length_error("SignalPdu: data exceeds the 16-bit Data Length (8191 octets)");
    }
    return static_cast<unsigned short>(_data.size() * 8);
}

void SignalPdu::setDataLength(unsigned short pX)
{
    if(octetsForBits(pX) != _data.size())
    {
        throw std::length_error("SignalPdu: Data Length does not describe the data octets");
    }
    _dataLength = pX;
}

unsigned short SignalPdu::getSamples() const
{
    return _samples;
}

void SignalPdu::setSamples(unsigned short pX)
{
    _samples = pX;
}

std::vector<uint8_t>& SignalPdu::getData()
{
    return _data;
}

const std::vector<uint8_t>& SignalPdu::getData() const
{
    return _data;
}

void SignalPdu::setData(const std::vector<uint8_t>& pX)
{
    if(pX.size() >= kMaxDataOctets)
    {
        throw std::length_error("SignalPdu: data exceeds the 16-bit Data Length (8191 octets)");
    }
    _data = pX;
    _dataLength = static_cast<unsigned short>(pX.size() * 8);
}

void SignalPdu::marshal(DataStream& dataStream) const
{
    const unsigned short dataLength = getDataLength(); // throws before anything is written

    RadioCommunicationsFamilyPdu::marshal(dataStream); // Marshal information in superclass first
    dataStream << _encodingScheme;
    dataStream << _tdlType;
    dataStream << _sampleRate;
    dataStream << dataLength;
    dataStream << _samples;

    for(std::size_t idx = 0; idx < _data.size(); idx++)
    {
        dataStream << _data[idx];
    }

    const unsigned char zero = 0;
    for(std::size_t idx = paddingOctets(_data.size()); idx > 0; idx--)
    {
        dataStream << zero;
    }
}

void SignalPdu::unmarshal(DataStream& dataStream)
{
    RadioCommunicationsFamilyPdu::unmarshal(dataStream); // unmarshal information in superclass first
    dataStream >> _encodingScheme;
    dataStream >> _tdlType;
    dataStream >> _sampleRate;
    dataStream >> _dataLength;
    dataStream >> _samples;

    // K need not be a whole number of octets; the last octet then carries
    // K % 8 valid bits.
    _data.clear();
    _data.resize(octetsForBits(_dataLength));
    for(std::size_t idx = 0; idx < _data.size(); idx++)
    {
        dataStream >> _data[idx];
    }

    unsigned char padding = 0;
    for(std::size_t idx = paddingOctets(_data.size()); idx > 0; idx--)
    {
        dataStream >> padding;
    }
}


bool SignalPdu::operator ==(const SignalPdu& rhs) const
 {
     bool ivarsEqual = true;

     ivarsEqual = RadioCommunicationsFamilyPdu::operator==(rhs);

     if( ! (_encodingScheme == rhs._encodingScheme) ) ivarsEqual = false;
     if( ! (_tdlType == rhs._tdlType) ) ivarsEqual = false;
     if( ! (_sampleRate == rhs._sampleRate) ) ivarsEqual = false;
     if( ! (_samples == rhs._samples) ) ivarsEqual = false;
     if( ! (_data == rhs._data) ) ivarsEqual = false;
     // Compare Data Length the way marshal derives it, without its throw for
     // oversized data: comparison never throws.
     if( ! (effectiveDataLength(_dataLength, _data.size()) ==
            effectiveDataLength(rhs._dataLength, rhs._data.size())) ) ivarsEqual = false;

    return ivarsEqual;
 }

int SignalPdu::getMarshalledSize() const
{
   int marshalSize = 0;

   marshalSize = RadioCommunicationsFamilyPdu::getMarshalledSize();
   marshalSize = marshalSize + 2;  // _encodingScheme
   marshalSize = marshalSize + 2;  // _tdlType
   marshalSize = marshalSize + 4;  // _sampleRate
   marshalSize = marshalSize + 2;  // _dataLength
   marshalSize = marshalSize + 2;  // _samples
   marshalSize = marshalSize + static_cast<int>(_data.size());  // _data
   marshalSize = marshalSize + static_cast<int>(paddingOctets(_data.size()));  // padding
    return marshalSize;
}
