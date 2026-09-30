#include <dis7/AggregateStatePdu.h>

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

// Aggregate ID and Entity ID records are both 48 bits.
const std::size_t kIdRecordOctets = 6;

// The Aggregate ID and Entity ID lists together are padded to a 32-bit
// boundary: 2 octets when W + J is odd.
std::size_t idListPaddingOctets(std::size_t idRecords)
{
    return (idRecords * kIdRecordOctets) % 4 == 0 ? 0 : 2;
}
}


AggregateStatePdu::AggregateStatePdu() : EntityManagementFamilyPdu(),
   _aggregateID(),
   _forceID(0),
   _aggregateState(0),
   _aggregateType(),
   _formation(0),
   _aggregateMarking(),
   _dimensions(),
   _orientation(),
   _centerOfMass(),
   _velocity(),
   _aggregateIDList(),
   _entityIDList(),
   _silentAggregateSystemList(),
   _silentEntitySystemList(),
   _variableDatumList()
{
    setPduType( 33 );
}

AggregateStatePdu::~AggregateStatePdu()
{
}

AggregateIdentifier& AggregateStatePdu::getAggregateID()
{
    return _aggregateID;
}

const AggregateIdentifier& AggregateStatePdu::getAggregateID() const
{
    return _aggregateID;
}

void AggregateStatePdu::setAggregateID(const AggregateIdentifier &pX)
{
    _aggregateID = pX;
}

unsigned char AggregateStatePdu::getForceID() const
{
    return _forceID;
}

void AggregateStatePdu::setForceID(unsigned char pX)
{
    _forceID = pX;
}

unsigned char AggregateStatePdu::getAggregateState() const
{
    return _aggregateState;
}

void AggregateStatePdu::setAggregateState(unsigned char pX)
{
    _aggregateState = pX;
}

AggregateType& AggregateStatePdu::getAggregateType()
{
    return _aggregateType;
}

const AggregateType& AggregateStatePdu::getAggregateType() const
{
    return _aggregateType;
}

void AggregateStatePdu::setAggregateType(const AggregateType &pX)
{
    _aggregateType = pX;
}

unsigned int AggregateStatePdu::getFormation() const
{
    return _formation;
}

void AggregateStatePdu::setFormation(unsigned int pX)
{
    _formation = pX;
}

AggregateMarking& AggregateStatePdu::getAggregateMarking()
{
    return _aggregateMarking;
}

const AggregateMarking& AggregateStatePdu::getAggregateMarking() const
{
    return _aggregateMarking;
}

void AggregateStatePdu::setAggregateMarking(const AggregateMarking &pX)
{
    _aggregateMarking = pX;
}

Vector3Float& AggregateStatePdu::getDimensions()
{
    return _dimensions;
}

const Vector3Float& AggregateStatePdu::getDimensions() const
{
    return _dimensions;
}

void AggregateStatePdu::setDimensions(const Vector3Float &pX)
{
    _dimensions = pX;
}

EulerAngles& AggregateStatePdu::getOrientation()
{
    return _orientation;
}

const EulerAngles& AggregateStatePdu::getOrientation() const
{
    return _orientation;
}

void AggregateStatePdu::setOrientation(const EulerAngles &pX)
{
    _orientation = pX;
}

Vector3Double& AggregateStatePdu::getCenterOfMass()
{
    return _centerOfMass;
}

const Vector3Double& AggregateStatePdu::getCenterOfMass() const
{
    return _centerOfMass;
}

void AggregateStatePdu::setCenterOfMass(const Vector3Double &pX)
{
    _centerOfMass = pX;
}

Vector3Float& AggregateStatePdu::getVelocity()
{
    return _velocity;
}

const Vector3Float& AggregateStatePdu::getVelocity() const
{
    return _velocity;
}

void AggregateStatePdu::setVelocity(const Vector3Float &pX)
{
    _velocity = pX;
}

unsigned short AggregateStatePdu::getNumberOfDisAggregates() const
{
   return checkedCount<unsigned short>(_aggregateIDList.size(),
       "AggregateStatePdu: more Aggregate IDs than the 16-bit count holds");
}

unsigned short AggregateStatePdu::getNumberOfDisEntities() const
{
   return checkedCount<unsigned short>(_entityIDList.size(),
       "AggregateStatePdu: more Entity IDs than the 16-bit count holds");
}

unsigned short AggregateStatePdu::getNumberOfSilentAggregateTypes() const
{
   return checkedCount<unsigned short>(_silentAggregateSystemList.size(),
       "AggregateStatePdu: more Silent Aggregate Systems than the 16-bit count holds");
}

unsigned short AggregateStatePdu::getNumberOfSilentEntityTypes() const
{
   return checkedCount<unsigned short>(_silentEntitySystemList.size(),
       "AggregateStatePdu: more Silent Entity Systems than the 16-bit count holds");
}

std::vector<AggregateIdentifier>& AggregateStatePdu::getAggregateIDList()
{
    return _aggregateIDList;
}

const std::vector<AggregateIdentifier>& AggregateStatePdu::getAggregateIDList() const
{
    return _aggregateIDList;
}

void AggregateStatePdu::setAggregateIDList(const std::vector<AggregateIdentifier>& pX)
{
     _aggregateIDList = pX;
}

std::vector<EntityID>& AggregateStatePdu::getEntityIDList()
{
    return _entityIDList;
}

const std::vector<EntityID>& AggregateStatePdu::getEntityIDList() const
{
    return _entityIDList;
}

void AggregateStatePdu::setEntityIDList(const std::vector<EntityID>& pX)
{
     _entityIDList = pX;
}

std::vector<SilentAggregateSystem>& AggregateStatePdu::getSilentAggregateSystemList()
{
    return _silentAggregateSystemList;
}

const std::vector<SilentAggregateSystem>& AggregateStatePdu::getSilentAggregateSystemList() const
{
    return _silentAggregateSystemList;
}

void AggregateStatePdu::setSilentAggregateSystemList(const std::vector<SilentAggregateSystem>& pX)
{
     _silentAggregateSystemList = pX;
}

std::vector<SilentEntitySystem>& AggregateStatePdu::getSilentEntitySystemList()
{
    return _silentEntitySystemList;
}

const std::vector<SilentEntitySystem>& AggregateStatePdu::getSilentEntitySystemList() const
{
    return _silentEntitySystemList;
}

void AggregateStatePdu::setSilentEntitySystemList(const std::vector<SilentEntitySystem>& pX)
{
     _silentEntitySystemList = pX;
}

unsigned int AggregateStatePdu::getNumberOfVariableDatumRecords() const
{
   return checkedCount<unsigned int>(_variableDatumList.size(),
       "AggregateStatePdu: more Variable Datum records than the 32-bit count holds");
}

std::vector<VariableDatum>& AggregateStatePdu::getVariableDatumList()
{
    return _variableDatumList;
}

const std::vector<VariableDatum>& AggregateStatePdu::getVariableDatumList() const
{
    return _variableDatumList;
}

void AggregateStatePdu::setVariableDatumList(const std::vector<VariableDatum>& pX)
{
     _variableDatumList = pX;
}

void AggregateStatePdu::marshal(DataStream& dataStream) const
{
    // The count getters throw for a list larger than its field; take the
    // counts before anything is written.
    const unsigned short numberOfDisAggregates = getNumberOfDisAggregates();
    const unsigned short numberOfDisEntities = getNumberOfDisEntities();
    const unsigned short numberOfSilentAggregateTypes = getNumberOfSilentAggregateTypes();
    const unsigned short numberOfSilentEntityTypes = getNumberOfSilentEntityTypes();
    const unsigned int numberOfVariableDatumRecords = getNumberOfVariableDatumRecords();

    EntityManagementFamilyPdu::marshal(dataStream); // Marshal information in superclass first
    _aggregateID.marshal(dataStream);
    dataStream << _forceID;
    dataStream << _aggregateState;
    _aggregateType.marshal(dataStream);
    dataStream << _formation;
    _aggregateMarking.marshal(dataStream);
    _dimensions.marshal(dataStream);
    _orientation.marshal(dataStream);
    _centerOfMass.marshal(dataStream);
    _velocity.marshal(dataStream);
    dataStream << numberOfDisAggregates;
    dataStream << numberOfDisEntities;
    dataStream << numberOfSilentAggregateTypes;
    dataStream << numberOfSilentEntityTypes;

    for(std::size_t idx = 0; idx < _aggregateIDList.size(); idx++)
    {
        _aggregateIDList[idx].marshal(dataStream);
    }

    for(std::size_t idx = 0; idx < _entityIDList.size(); idx++)
    {
        _entityIDList[idx].marshal(dataStream);
    }

    const unsigned char zero = 0;
    for(std::size_t idx = idListPaddingOctets(_aggregateIDList.size() + _entityIDList.size()); idx > 0; idx--)
    {
        dataStream << zero;
    }

    for(std::size_t idx = 0; idx < _silentAggregateSystemList.size(); idx++)
    {
        _silentAggregateSystemList[idx].marshal(dataStream);
    }

    for(std::size_t idx = 0; idx < _silentEntitySystemList.size(); idx++)
    {
        _silentEntitySystemList[idx].marshal(dataStream);
    }

    dataStream << numberOfVariableDatumRecords;

    for(std::size_t idx = 0; idx < _variableDatumList.size(); idx++)
    {
        _variableDatumList[idx].marshal(dataStream);
    }
}

void AggregateStatePdu::unmarshal(DataStream& dataStream)
{
    EntityManagementFamilyPdu::unmarshal(dataStream); // unmarshal information in superclass first
    _aggregateID.unmarshal(dataStream);
    dataStream >> _forceID;
    dataStream >> _aggregateState;
    _aggregateType.unmarshal(dataStream);
    dataStream >> _formation;
    _aggregateMarking.unmarshal(dataStream);
    _dimensions.unmarshal(dataStream);
    _orientation.unmarshal(dataStream);
    _centerOfMass.unmarshal(dataStream);
    _velocity.unmarshal(dataStream);

    unsigned short numberOfDisAggregates = 0;
    unsigned short numberOfDisEntities = 0;
    unsigned short numberOfSilentAggregateTypes = 0;
    unsigned short numberOfSilentEntityTypes = 0;
    dataStream >> numberOfDisAggregates;
    dataStream >> numberOfDisEntities;
    dataStream >> numberOfSilentAggregateTypes;
    dataStream >> numberOfSilentEntityTypes;

    _aggregateIDList.clear();
    for(std::size_t idx = 0; idx < numberOfDisAggregates; idx++)
    {
        AggregateIdentifier x;
        x.unmarshal(dataStream);
        _aggregateIDList.push_back(x);
    }

    _entityIDList.clear();
    for(std::size_t idx = 0; idx < numberOfDisEntities; idx++)
    {
        EntityID x;
        x.unmarshal(dataStream);
        _entityIDList.push_back(x);
    }

    unsigned char padding = 0;
    for(std::size_t idx = idListPaddingOctets(_aggregateIDList.size() + _entityIDList.size()); idx > 0; idx--)
    {
        dataStream >> padding;
    }

    _silentAggregateSystemList.clear();
    for(std::size_t idx = 0; idx < numberOfSilentAggregateTypes; idx++)
    {
        SilentAggregateSystem x;
        x.unmarshal(dataStream);
        _silentAggregateSystemList.push_back(x);
    }

    _silentEntitySystemList.clear();
    for(std::size_t idx = 0; idx < numberOfSilentEntityTypes; idx++)
    {
        SilentEntitySystem x;
        x.unmarshal(dataStream);
        _silentEntitySystemList.push_back(x);
    }

    unsigned int numberOfVariableDatumRecords = 0;
    dataStream >> numberOfVariableDatumRecords;

    _variableDatumList.clear();
    for(std::size_t idx = 0; idx < numberOfVariableDatumRecords; idx++)
    {
        VariableDatum x;
        x.unmarshal(dataStream);
        _variableDatumList.push_back(x);
    }
}


bool AggregateStatePdu::operator ==(const AggregateStatePdu& rhs) const
 {
     bool ivarsEqual = true;

     ivarsEqual = EntityManagementFamilyPdu::operator==(rhs);

     if( ! (_aggregateID == rhs._aggregateID) ) ivarsEqual = false;
     if( ! (_forceID == rhs._forceID) ) ivarsEqual = false;
     if( ! (_aggregateState == rhs._aggregateState) ) ivarsEqual = false;
     if( ! (_aggregateType == rhs._aggregateType) ) ivarsEqual = false;
     if( ! (_formation == rhs._formation) ) ivarsEqual = false;
     if( ! (_aggregateMarking == rhs._aggregateMarking) ) ivarsEqual = false;
     if( ! (_dimensions == rhs._dimensions) ) ivarsEqual = false;
     if( ! (_orientation == rhs._orientation) ) ivarsEqual = false;
     if( ! (_centerOfMass == rhs._centerOfMass) ) ivarsEqual = false;
     if( ! (_velocity == rhs._velocity) ) ivarsEqual = false;
     if( ! (_aggregateIDList == rhs._aggregateIDList) ) ivarsEqual = false;
     if( ! (_entityIDList == rhs._entityIDList) ) ivarsEqual = false;
     if( ! (_silentAggregateSystemList == rhs._silentAggregateSystemList) ) ivarsEqual = false;
     if( ! (_silentEntitySystemList == rhs._silentEntitySystemList) ) ivarsEqual = false;
     if( ! (_variableDatumList == rhs._variableDatumList) ) ivarsEqual = false;

    return ivarsEqual;
 }

int AggregateStatePdu::getMarshalledSize() const
{
   int marshalSize = 0;

   marshalSize = EntityManagementFamilyPdu::getMarshalledSize();
   marshalSize = marshalSize + _aggregateID.getMarshalledSize();  // _aggregateID
   marshalSize = marshalSize + 1;  // _forceID
   marshalSize = marshalSize + 1;  // _aggregateState
   marshalSize = marshalSize + _aggregateType.getMarshalledSize();  // _aggregateType
   marshalSize = marshalSize + 4;  // _formation
   marshalSize = marshalSize + _aggregateMarking.getMarshalledSize();  // _aggregateMarking
   marshalSize = marshalSize + _dimensions.getMarshalledSize();  // _dimensions
   marshalSize = marshalSize + _orientation.getMarshalledSize();  // _orientation
   marshalSize = marshalSize + _centerOfMass.getMarshalledSize();  // _centerOfMass
   marshalSize = marshalSize + _velocity.getMarshalledSize();  // _velocity
   marshalSize = marshalSize + 2;  // Number of Aggregate IDs
   marshalSize = marshalSize + 2;  // Number of Entity IDs
   marshalSize = marshalSize + 2;  // Number of Silent Aggregate Systems
   marshalSize = marshalSize + 2;  // Number of Silent Entity Systems

   for(std::size_t idx=0; idx < _aggregateIDList.size(); idx++)
   {
        marshalSize = marshalSize + _aggregateIDList[idx].getMarshalledSize();
   }

   for(std::size_t idx=0; idx < _entityIDList.size(); idx++)
   {
        marshalSize = marshalSize + _entityIDList[idx].getMarshalledSize();
   }

   marshalSize = marshalSize + static_cast<int>(idListPaddingOctets(_aggregateIDList.size() + _entityIDList.size()));

   for(std::size_t idx=0; idx < _silentAggregateSystemList.size(); idx++)
   {
        marshalSize = marshalSize + _silentAggregateSystemList[idx].getMarshalledSize();
   }

   for(std::size_t idx=0; idx < _silentEntitySystemList.size(); idx++)
   {
        marshalSize = marshalSize + _silentEntitySystemList[idx].getMarshalledSize();
   }

   marshalSize = marshalSize + 4;  // Number of Variable Datum Records

   for(std::size_t idx=0; idx < _variableDatumList.size(); idx++)
   {
        marshalSize = marshalSize + _variableDatumList[idx].getMarshalledSize();
   }

    return marshalSize;
}
