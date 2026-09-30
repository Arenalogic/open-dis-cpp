#pragma once

#include <dis7/RadioCommunicationsFamilyPdu.h>
#include <dis7/utils/DataStream.h>
#include <dis7/opendis7_export.h>
#include <cstdint>
#include <vector>


namespace DIS
{
// Signal PDU. IEEE 1278.1-2012 7.7.3, Table 178.
//
// The Radio Reference ID and Radio Number are the Entity ID and Radio ID of
// RadioCommunicationsFamilyPdu. The 32-octet fixed part is followed by the
// Data field (Data Length K bits, carried in ceil(K/8) octets) and zero padding
// to a 32-bit boundary.
//
// Data Length is a count of BITS and need not fill the last data octet. A
// stored Data Length stays authoritative while ceil(K/8) equals the data size
// (so a received PDU with a partial last octet re-marshals bit-exact);
// otherwise Data Length is the data size in octets * 8. Data of 8192 octets or
// more cannot be described by the 16-bit Data Length: setData and marshal throw
// std::length_error for it. The unused low-order bits of a partial last octet
// are padding: marshal writes them as zero (the stored data is not changed)
// and operator== ignores them.

class OPENDIS7_EXPORT SignalPdu : public RadioCommunicationsFamilyPdu
{
protected:
  /** encoding scheme record: encoding class and encoding type */
  unsigned short _encodingScheme;

  /** TDL type */
  unsigned short _tdlType;

  /** sample rate or data rate */
  unsigned int _sampleRate;

  /** data length, in bits (K) */
  unsigned short _dataLength;

  /** number of samples */
  unsigned short _samples;

  /** data, ceil(K/8) octets */
  std::vector<uint8_t> _data;


 public:
    SignalPdu();
    virtual ~SignalPdu();

    virtual void marshal(DataStream& dataStream) const;
    virtual void unmarshal(DataStream& dataStream);

    unsigned short getEncodingScheme() const;
    void setEncodingScheme(unsigned short pX);

    unsigned short getTdlType() const;
    void setTdlType(unsigned short pX);

    unsigned int getSampleRate() const;
    void setSampleRate(unsigned int pX);

    /** Data Length (K), in bits. Throws std::length_error when the data holds
        8192 octets or more and no stored K describes it. */
    unsigned short getDataLength() const;

    /** Sets a Data Length (K), in bits, that does not fill the last data octet.
        Throws std::length_error unless ceil(K/8) equals the data size, so call
        it after setData. */
    void setDataLength(unsigned short pX);

    unsigned short getSamples() const;
    void setSamples(unsigned short pX);

    std::vector<uint8_t>& getData();
    const std::vector<uint8_t>& getData() const;

    /** Sets the data and a Data Length of data size * 8 bits. Throws
        std::length_error for 8192 octets or more. */
    void setData(const std::vector<uint8_t>& pX);


virtual int getMarshalledSize() const;

     bool operator  ==(const SignalPdu& rhs) const;
};
}
