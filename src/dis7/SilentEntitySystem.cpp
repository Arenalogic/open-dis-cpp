#include <dis7/SilentEntitySystem.h>

#include <limits>
#include <stdexcept>

using namespace DIS;

namespace
{
// A container size as the value of its count field. Throws std::length_error
// when the size does not fit the field, instead of wrapping.
template <typename Count>
Count checkedCount(std::size_t size, const char* message)
{
    if(size > static_cast<std::size_t>(std::numeric_limits<Count>::max()))
    {
        throw std::length_error(message);
    }
    return static_cast<Count>(size);
}
}


SilentEntitySystem::SilentEntitySystem():
   _numberOfEntities(0),
   _entityType(),
   _appearanceRecordList()
{
}

SilentEntitySystem::~SilentEntitySystem()
{
}

unsigned short SilentEntitySystem::getNumberOfEntities() const
{
    return _numberOfEntities;
}

void SilentEntitySystem::setNumberOfEntities(unsigned short pX)
{
    _numberOfEntities = pX;
}

unsigned short SilentEntitySystem::getNumberOfAppearanceRecords() const
{
   return checkedCount<unsigned short>(_appearanceRecordList.size(),
       "SilentEntitySystem: more Entity Appearance records than the 16-bit count holds");
}

EntityType& SilentEntitySystem::getEntityType()
{
    return _entityType;
}

const EntityType& SilentEntitySystem::getEntityType() const
{
    return _entityType;
}

void SilentEntitySystem::setEntityType(const EntityType &pX)
{
    _entityType = pX;
}

std::vector<unsigned int>& SilentEntitySystem::getAppearanceRecordList()
{
    return _appearanceRecordList;
}

const std::vector<unsigned int>& SilentEntitySystem::getAppearanceRecordList() const
{
    return _appearanceRecordList;
}

void SilentEntitySystem::setAppearanceRecordList(const std::vector<unsigned int>& pX)
{
     _appearanceRecordList = pX;
}

void SilentEntitySystem::marshal(DataStream& dataStream) const
{
    // Throws for a list larger than its field, before anything is written.
    const unsigned short numberOfAppearanceRecords = getNumberOfAppearanceRecords();

    dataStream << _numberOfEntities;
    dataStream << numberOfAppearanceRecords;
    _entityType.marshal(dataStream);

    for(std::size_t idx = 0; idx < _appearanceRecordList.size(); idx++)
    {
        dataStream << _appearanceRecordList[idx];
    }
}

void SilentEntitySystem::unmarshal(DataStream& dataStream)
{
    unsigned short numberOfAppearanceRecords = 0;
    dataStream >> _numberOfEntities;
    dataStream >> numberOfAppearanceRecords;
    _entityType.unmarshal(dataStream);

    _appearanceRecordList.clear();
    _appearanceRecordList.resize(numberOfAppearanceRecords);
    for(std::size_t idx = 0; idx < _appearanceRecordList.size(); idx++)
    {
        dataStream >> _appearanceRecordList[idx];
    }
}


bool SilentEntitySystem::operator ==(const SilentEntitySystem& rhs) const
 {
     bool ivarsEqual = true;

     if( ! (_numberOfEntities == rhs._numberOfEntities) ) ivarsEqual = false;
     if( ! (_entityType == rhs._entityType) ) ivarsEqual = false;
     if( ! (_appearanceRecordList == rhs._appearanceRecordList) ) ivarsEqual = false;

    return ivarsEqual;
 }

int SilentEntitySystem::getMarshalledSize() const
{
   int marshalSize = 0;

   marshalSize = marshalSize + 2;  // _numberOfEntities
   marshalSize = marshalSize + 2;  // Number of Appearance Records
   marshalSize = marshalSize + _entityType.getMarshalledSize();  // _entityType
   marshalSize = marshalSize + 4 * static_cast<int>(_appearanceRecordList.size());  // _appearanceRecordList
    return marshalSize;
}
