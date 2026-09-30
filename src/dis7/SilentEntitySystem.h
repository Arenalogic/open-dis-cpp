#pragma once

#include <dis7/EntityType.h>
#include <dis7/utils/DataStream.h>
#include <dis7/opendis7_export.h>
#include <vector>


namespace DIS
{
// Silent Entity System record. IEEE 1278.1-2012 6.2.79, Table 110: Number of
// Entities (16-bit), Number of Appearance Records (A, 16-bit), Entity Type
// record, then A 32-bit Entity Appearance records. 96 + 32A bits.
//
// A is derived from the appearance record list. getNumberOfAppearanceRecords,
// and so marshal, throw std::length_error when the list does not fit the
// 16-bit count.

class OPENDIS7_EXPORT SilentEntitySystem
{
protected:
  /** number of entities of this type */
  unsigned short _numberOfEntities;

  /** entity type common to the entities in this system list */
  EntityType _entityType;

  /** Entity Appearance records (32 bits each) that deviate from the default */
  std::vector<unsigned int> _appearanceRecordList;


 public:
    SilentEntitySystem();
    virtual ~SilentEntitySystem();

    virtual void marshal(DataStream& dataStream) const;
    virtual void unmarshal(DataStream& dataStream);

    unsigned short getNumberOfEntities() const;
    void setNumberOfEntities(unsigned short pX);

    unsigned short getNumberOfAppearanceRecords() const;

    EntityType& getEntityType();
    const EntityType&  getEntityType() const;
    void setEntityType(const EntityType    &pX);

    std::vector<unsigned int>& getAppearanceRecordList();
    const std::vector<unsigned int>& getAppearanceRecordList() const;
    void setAppearanceRecordList(const std::vector<unsigned int>&    pX);


virtual int getMarshalledSize() const;

     bool operator  ==(const SilentEntitySystem& rhs) const;
};
}
