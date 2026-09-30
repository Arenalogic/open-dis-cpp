#pragma once

#include <dis7/AggregateType.h>
#include <dis7/utils/DataStream.h>
#include <dis7/opendis7_export.h>


namespace DIS
{
// Silent Aggregate System. IEEE 1278.1-2012 7.8.2 item r) and Table 183: Number
// of Aggregates (16-bit), 16 bits of padding, Aggregate Type record. 96 bits.

class OPENDIS7_EXPORT SilentAggregateSystem
{
protected:
  /** number of aggregates of this type */
  unsigned short _numberOfAggregates;

  /** aggregate type common to this system list */
  AggregateType _aggregateType;


 public:
    SilentAggregateSystem();
    virtual ~SilentAggregateSystem();

    virtual void marshal(DataStream& dataStream) const;
    virtual void unmarshal(DataStream& dataStream);

    unsigned short getNumberOfAggregates() const;
    void setNumberOfAggregates(unsigned short pX);

    AggregateType& getAggregateType();
    const AggregateType&  getAggregateType() const;
    void setAggregateType(const AggregateType    &pX);


virtual int getMarshalledSize() const;

     bool operator  ==(const SilentAggregateSystem& rhs) const;
};
}
