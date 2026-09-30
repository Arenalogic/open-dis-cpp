#include <dis7/SilentEntitySystem.h>

#include <stdexcept>

using namespace DIS;


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
   return static_cast<unsigned short>(_appearanceRecordList.size());
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
    if(_appearanceRecordList.size() > 0xFFFF)
    {
        throw std::length_error("SilentEntitySystem: more Entity Appearance records than the 16-bit count holds");
    }

    dataStream << _numberOfEntities;
    dataStream << getNumberOfAppearanceRecords();
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
