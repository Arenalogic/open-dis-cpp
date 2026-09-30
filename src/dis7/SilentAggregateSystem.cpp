#include <dis7/SilentAggregateSystem.h>

using namespace DIS;


SilentAggregateSystem::SilentAggregateSystem():
   _numberOfAggregates(0),
   _aggregateType()
{
}

SilentAggregateSystem::~SilentAggregateSystem()
{
}

unsigned short SilentAggregateSystem::getNumberOfAggregates() const
{
    return _numberOfAggregates;
}

void SilentAggregateSystem::setNumberOfAggregates(unsigned short pX)
{
    _numberOfAggregates = pX;
}

AggregateType& SilentAggregateSystem::getAggregateType()
{
    return _aggregateType;
}

const AggregateType& SilentAggregateSystem::getAggregateType() const
{
    return _aggregateType;
}

void SilentAggregateSystem::setAggregateType(const AggregateType &pX)
{
    _aggregateType = pX;
}

void SilentAggregateSystem::marshal(DataStream& dataStream) const
{
    const unsigned short padding = 0;
    dataStream << _numberOfAggregates;
    dataStream << padding;
    _aggregateType.marshal(dataStream);
}

void SilentAggregateSystem::unmarshal(DataStream& dataStream)
{
    unsigned short padding = 0;
    dataStream >> _numberOfAggregates;
    dataStream >> padding;
    _aggregateType.unmarshal(dataStream);
}


bool SilentAggregateSystem::operator ==(const SilentAggregateSystem& rhs) const
 {
     bool ivarsEqual = true;

     if( ! (_numberOfAggregates == rhs._numberOfAggregates) ) ivarsEqual = false;
     if( ! (_aggregateType == rhs._aggregateType) ) ivarsEqual = false;

    return ivarsEqual;
 }

int SilentAggregateSystem::getMarshalledSize() const
{
   int marshalSize = 0;

   marshalSize = marshalSize + 2;  // _numberOfAggregates
   marshalSize = marshalSize + 2;  // padding
   marshalSize = marshalSize + _aggregateType.getMarshalledSize();  // _aggregateType
    return marshalSize;
}
