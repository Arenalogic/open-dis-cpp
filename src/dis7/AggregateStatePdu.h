#pragma once

#include <dis7/AggregateIdentifier.h>
#include <dis7/AggregateType.h>
#include <dis7/AggregateMarking.h>
#include <dis7/Vector3Float.h>
#include <dis7/EulerAngles.h>
#include <dis7/Vector3Double.h>
#include <dis7/EntityID.h>
#include <dis7/SilentAggregateSystem.h>
#include <dis7/SilentEntitySystem.h>
#include <dis7/VariableDatum.h>
#include <vector>
#include <dis7/EntityManagementFamilyPdu.h>
#include <dis7/utils/DataStream.h>
#include <dis7/opendis7_export.h>


namespace DIS
{
// Aggregate State PDU. IEEE 1278.1-2012 7.8.2, Table 183.
//
// The 132-octet fixed part (header through the W, J, N and S counts) is
// followed by W Aggregate IDs, J Entity IDs, padding to a 32-bit boundary
// (2 octets when W + J is odd), N Silent Aggregate Systems, S Silent Entity
// Systems, the Number of Variable Datum Records (M, 32-bit) and M Variable
// Datum records. All counts are derived from the list sizes on marshal;
// marshal throws std::length_error when a list does not fit its 16-bit count.

class OPENDIS7_EXPORT AggregateStatePdu : public EntityManagementFamilyPdu
{
protected:
  /** ID of aggregated entities */
  AggregateIdentifier _aggregateID;

  /** force ID */
  unsigned char _forceID;

  /** state of aggregate */
  unsigned char _aggregateState;

  /** aggregate type */
  AggregateType _aggregateType;

  /** formation of aggregated entities */
  unsigned int _formation;

  /** marking for aggregate; first char is charset type, rest is char data */
  AggregateMarking _aggregateMarking;

  /** dimensions of bounding box for the aggregated entities, origin at the center of mass */
  Vector3Float _dimensions;

  /** orientation of the bounding box */
  EulerAngles _orientation;

  /** center of mass of the aggregation */
  Vector3Double _centerOfMass;

  /** velocity of aggregation */
  Vector3Float _velocity;

  /** aggregates that publish Aggregate State PDUs (W) */
  std::vector<AggregateIdentifier> _aggregateIDList;

  /** entities that publish Entity State PDUs (J) */
  std::vector<EntityID> _entityIDList;

  /** subaggregates that do not publish Aggregate State PDUs (N) */
  std::vector<SilentAggregateSystem> _silentAggregateSystemList;

  /** entity systems that do not publish Entity State PDUs (S) */
  std::vector<SilentEntitySystem> _silentEntitySystemList;

  /** variable datum records (M) */
  std::vector<VariableDatum> _variableDatumList;


 public:
    AggregateStatePdu();
    virtual ~AggregateStatePdu();

    virtual void marshal(DataStream& dataStream) const;
    virtual void unmarshal(DataStream& dataStream);

    AggregateIdentifier& getAggregateID();
    const AggregateIdentifier&  getAggregateID() const;
    void setAggregateID(const AggregateIdentifier    &pX);

    unsigned char getForceID() const;
    void setForceID(unsigned char pX);

    unsigned char getAggregateState() const;
    void setAggregateState(unsigned char pX);

    AggregateType& getAggregateType();
    const AggregateType&  getAggregateType() const;
    void setAggregateType(const AggregateType    &pX);

    unsigned int getFormation() const;
    void setFormation(unsigned int pX);

    AggregateMarking& getAggregateMarking();
    const AggregateMarking&  getAggregateMarking() const;
    void setAggregateMarking(const AggregateMarking    &pX);

    Vector3Float& getDimensions();
    const Vector3Float&  getDimensions() const;
    void setDimensions(const Vector3Float    &pX);

    EulerAngles& getOrientation();
    const EulerAngles&  getOrientation() const;
    void setOrientation(const EulerAngles    &pX);

    Vector3Double& getCenterOfMass();
    const Vector3Double&  getCenterOfMass() const;
    void setCenterOfMass(const Vector3Double    &pX);

    Vector3Float& getVelocity();
    const Vector3Float&  getVelocity() const;
    void setVelocity(const Vector3Float    &pX);

    /** Number of Aggregate IDs (W) */
    unsigned short getNumberOfDisAggregates() const;

    /** Number of Entity IDs (J) */
    unsigned short getNumberOfDisEntities() const;

    /** Number of Silent Aggregate Systems (N) */
    unsigned short getNumberOfSilentAggregateTypes() const;

    /** Number of Silent Entity Systems (S) */
    unsigned short getNumberOfSilentEntityTypes() const;

    std::vector<AggregateIdentifier>& getAggregateIDList();
    const std::vector<AggregateIdentifier>& getAggregateIDList() const;
    void setAggregateIDList(const std::vector<AggregateIdentifier>&    pX);

    std::vector<EntityID>& getEntityIDList();
    const std::vector<EntityID>& getEntityIDList() const;
    void setEntityIDList(const std::vector<EntityID>&    pX);

    std::vector<SilentAggregateSystem>& getSilentAggregateSystemList();
    const std::vector<SilentAggregateSystem>& getSilentAggregateSystemList() const;
    void setSilentAggregateSystemList(const std::vector<SilentAggregateSystem>&    pX);

    std::vector<SilentEntitySystem>& getSilentEntitySystemList();
    const std::vector<SilentEntitySystem>& getSilentEntitySystemList() const;
    void setSilentEntitySystemList(const std::vector<SilentEntitySystem>&    pX);

    /** Number of Variable Datum Records (M) */
    unsigned int getNumberOfVariableDatumRecords() const;

    std::vector<VariableDatum>& getVariableDatumList();
    const std::vector<VariableDatum>& getVariableDatumList() const;
    void setVariableDatumList(const std::vector<VariableDatum>&    pX);


virtual int getMarshalledSize() const;

     bool operator  ==(const AggregateStatePdu& rhs) const;
};
}
