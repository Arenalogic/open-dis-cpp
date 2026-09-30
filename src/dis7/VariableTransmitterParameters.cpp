#include <dis7/VariableTransmitterParameters.h>

#include <stdexcept>

using namespace DIS;

namespace
{
// Record Type (4 octets) + Record Length (2 octets)
const std::size_t kHeaderOctets = 6;

// Zero octets that end a record holding `recordSpecificOctets` on a 64-bit boundary.
std::size_t paddingOctets(std::size_t recordSpecificOctets)
{
    return (8 - (kHeaderOctets + recordSpecificOctets) % 8) % 8;
}

std::size_t totalOctets(std::size_t recordSpecificOctets)
{
    return kHeaderOctets + recordSpecificOctets + paddingOctets(recordSpecificOctets);
}
}


VariableTransmitterParameters::VariableTransmitterParameters():
   _recordType(0), 
   _recordSpecificFields()
{
}

VariableTransmitterParameters::~VariableTransmitterParameters()
{
}

unsigned int VariableTransmitterParameters::getRecordType() const
{
    return _recordType;
}

void VariableTransmitterParameters::setRecordType(unsigned int pX)
{
    _recordType = pX;
}

unsigned short VariableTransmitterParameters::getRecordLength() const
{
    const std::size_t length = totalOctets(_recordSpecificFields.size());
    if(length > 0xFFFF)
    {
        throw std::length_error("VariableTransmitterParameters: record exceeds the 16-bit Record Length");
    }
    return static_cast<unsigned short>(length);
}

std::vector<unsigned char>& VariableTransmitterParameters::getRecordSpecificFields()
{
    return _recordSpecificFields;
}

const std::vector<unsigned char>& VariableTransmitterParameters::getRecordSpecificFields() const
{
    return _recordSpecificFields;
}

void VariableTransmitterParameters::setRecordSpecificFields(const std::vector<unsigned char>& pX)
{
    _recordSpecificFields = pX;
}

void VariableTransmitterParameters::marshal(DataStream& dataStream) const
{
    const unsigned short recordLength = getRecordLength(); // throws before anything is written
    dataStream << _recordType;
    dataStream << recordLength;

    for(std::size_t idx = 0; idx < _recordSpecificFields.size(); idx++)
    {
        dataStream << _recordSpecificFields[idx];
    }

    const unsigned char zero = 0;
    for(std::size_t idx = paddingOctets(_recordSpecificFields.size()); idx > 0; idx--)
    {
        dataStream << zero;
    }
}

void VariableTransmitterParameters::unmarshal(DataStream& dataStream)
{
    unsigned int recordType = 0;
    unsigned short recordLength = 0;
    dataStream >> recordType;
    dataStream >> recordLength;

    // The record ends on a 64-bit boundary: Record Length is a multiple of 8
    // and at least 8 (the header plus its padding).
    if(recordLength < 8 || recordLength % 8 != 0)
    {
        throw std::length_error("VariableTransmitterParameters: Record Length is not a positive multiple of 8 octets");
    }

    // Refuse a length the input cannot hold before sizing the fields for it.
    // std::out_of_range is what DataStream throws for a read past its end.
    const std::size_t recordSpecificOctets = recordLength - kHeaderOctets;
    if(recordSpecificOctets > dataStream.size() - dataStream.GetReadPos())
    {
        throw std::out_of_range("VariableTransmitterParameters: Record Length exceeds the remaining input");
    }

    _recordType = recordType;
    _recordSpecificFields.clear();
    _recordSpecificFields.resize(recordSpecificOctets);
    for(std::size_t idx = 0; idx < _recordSpecificFields.size(); idx++)
    {
        dataStream >> _recordSpecificFields[idx];
    }
}


bool VariableTransmitterParameters::operator ==(const VariableTransmitterParameters& rhs) const
 {
     bool ivarsEqual = true;

     if( ! (_recordType == rhs._recordType) ) ivarsEqual = false;

     // Compare the record-specific fields as marshal writes them, zero-padded
     // to the 64-bit boundary: a record and its own round trip (which stores
     // the padding octets) compare equal.
     const std::size_t lhsOctets = _recordSpecificFields.size();
     const std::size_t rhsOctets = rhs._recordSpecificFields.size();
     const std::size_t paddedOctets = lhsOctets + paddingOctets(lhsOctets);
     if( ! (paddedOctets == rhsOctets + paddingOctets(rhsOctets)) ) ivarsEqual = false;
     else
     {
         for(std::size_t idx = 0; idx < paddedOctets; idx++)
         {
             const unsigned char lhs = idx < lhsOctets ? _recordSpecificFields[idx] : 0;
             const unsigned char rhsOctet = idx < rhsOctets ? rhs._recordSpecificFields[idx] : 0;
             if( ! (lhs == rhsOctet) ) { ivarsEqual = false; break; }
         }
     }

    return ivarsEqual;
 }

int VariableTransmitterParameters::getMarshalledSize() const
{
   return static_cast<int>(totalOctets(_recordSpecificFields.size()));
}

// Copyright (c) 1995-2009 held by the author(s).  All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
//  are met:
// 
//  * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// * Neither the names of the Naval Postgraduate School (NPS)
//  Modeling Virtual Environments and Simulation (MOVES) Institute
// (http://www.nps.edu and http://www.MovesInstitute.org)
// nor the names of its contributors may be used to endorse or
//  promote products derived from this software without specific
// prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// AS IS AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
