#pragma once

#include <dis7/utils/DataStream.h>
#include <dis7/opendis7_export.h>
#include <vector>


namespace DIS
{
// Variable Transmitter Parameters (VTP) record. IEEE 1278.1-2012 6.2.95, Table 130:
// Record Type (32-bit enumeration), Record Length (16-bit, total octets in the
// record = 6 + K + P), K octets of record-specific fields, then P padding octets
// so the record ends on a 64-bit boundary.
//
// The record-specific octets are stored raw. The stored octets may already
// include the trailing padding; marshal appends zero octets as needed so the
// record always ends on a 64-bit boundary. Record Length is derived from the
// stored octets. unmarshal stores all Record Length - 6 octets that follow the
// header, padding included; it throws std::length_error for a Record Length
// that is not a positive multiple of 8, and std::out_of_range for one the
// remaining input cannot hold.

// Copyright (c) 2007-2009, MOVES Institute, Naval Postgraduate School. All rights reserved. 
//
// @author DMcG, jkg

class OPENDIS7_EXPORT VariableTransmitterParameters
{
protected:
  /** Type of VTP. Enumeration from EBV */
  unsigned int _recordType; 

  /** Record-specific fields, raw, possibly including trailing padding */
  std::vector<unsigned char> _recordSpecificFields;


 public:
    VariableTransmitterParameters();
    virtual ~VariableTransmitterParameters();

    virtual void marshal(DataStream& dataStream) const;
    virtual void unmarshal(DataStream& dataStream);

    unsigned int getRecordType() const; 
    void setRecordType(unsigned int pX); 

    /** Total record length in octets (6 + K + P), derived from the stored octets.
        Throws std::length_error when the record does not fit the 16-bit field. */
    unsigned short getRecordLength() const; 

    std::vector<unsigned char>& getRecordSpecificFields(); 
    const std::vector<unsigned char>& getRecordSpecificFields() const; 
    void setRecordSpecificFields(const std::vector<unsigned char>& pX);


virtual int getMarshalledSize() const;

     bool operator  ==(const VariableTransmitterParameters& rhs) const;
};
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
