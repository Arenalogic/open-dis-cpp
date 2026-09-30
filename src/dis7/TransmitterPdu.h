#pragma once

#include <dis7/RadioType.h>
#include <dis7/Vector3Double.h>
#include <dis7/Vector3Float.h>
#include <dis7/ModulationType.h>
#include <dis7/VariableTransmitterParameters.h>
#include <vector>
#include <dis7/RadioCommunicationsFamilyPdu.h>
#include <dis7/utils/DataStream.h>
#include <dis7/opendis7_export.h>


namespace DIS
{
// Transmitter PDU. IEEE 1278.1-2012 7.7.2, Table 176.
//
// The Radio Reference ID and Radio Number are the Entity ID and Radio ID of
// RadioCommunicationsFamilyPdu. The fixed part is 104 octets. It is followed by
// the parameter records section: the Modulation Parameters record (M octets,
// raw), the Antenna Pattern record (A octets, raw) and the Variable Transmitter
// Parameters records. M, A and the VTP record count are derived from the
// container sizes on marshal; marshal throws std::length_error when a container
// does not fit its count field.

class OPENDIS7_EXPORT TransmitterPdu : public RadioCommunicationsFamilyPdu
{
protected:
  /** type of radio */
  RadioType _radioEntityType;

  /** transmit state */
  unsigned char _transmitState;

  /** input source */
  unsigned char _inputSource;

  /** location of the radiating portion of the antenna, world coordinates */
  Vector3Double _antennaLocation;

  /** location of the antenna relative to the entity */
  Vector3Float _relativeAntennaLocation;

  /** antenna pattern type */
  unsigned short _antennaPatternType;

  /** center frequency, in Hz */
  unsigned long long _frequency;

  /** transmit frequency bandwidth, in Hz */
  float _transmitFrequencyBandwidth;

  /** power, in dBm */
  float _power;

  /** modulation type */
  ModulationType _modulationType;

  /** crypto system enumeration */
  unsigned short _cryptoSystem;

  /** crypto key ID record */
  unsigned short _cryptoKeyId;

  /** Modulation Parameters record, raw octets (M) */
  std::vector<unsigned char> _modulationParametersList;

  /** Antenna Pattern record, raw octets (A) */
  std::vector<unsigned char> _antennaPatternList;

  /** Variable Transmitter Parameters records (N) */
  std::vector<VariableTransmitterParameters> _variableTransmitterParametersList;


 public:
    TransmitterPdu();
    virtual ~TransmitterPdu();

    virtual void marshal(DataStream& dataStream) const;
    virtual void unmarshal(DataStream& dataStream);

    RadioType& getRadioEntityType();
    const RadioType&  getRadioEntityType() const;
    void setRadioEntityType(const RadioType    &pX);

    unsigned char getTransmitState() const;
    void setTransmitState(unsigned char pX);

    unsigned char getInputSource() const;
    void setInputSource(unsigned char pX);

    unsigned short getVariableTransmitterParameterCount() const;

    Vector3Double& getAntennaLocation();
    const Vector3Double&  getAntennaLocation() const;
    void setAntennaLocation(const Vector3Double    &pX);

    Vector3Float& getRelativeAntennaLocation();
    const Vector3Float&  getRelativeAntennaLocation() const;
    void setRelativeAntennaLocation(const Vector3Float    &pX);

    unsigned short getAntennaPatternType() const;
    void setAntennaPatternType(unsigned short pX);

    /** Antenna Pattern Length (A), in octets */
    unsigned short getAntennaPatternCount() const;

    unsigned long long getFrequency() const;
    void setFrequency(unsigned long long pX);

    float getTransmitFrequencyBandwidth() const;
    void setTransmitFrequencyBandwidth(float pX);

    float getPower() const;
    void setPower(float pX);

    ModulationType& getModulationType();
    const ModulationType&  getModulationType() const;
    void setModulationType(const ModulationType    &pX);

    unsigned short getCryptoSystem() const;
    void setCryptoSystem(unsigned short pX);

    unsigned short getCryptoKeyId() const;
    void setCryptoKeyId(unsigned short pX);

    /** Length of Modulation Parameters (M), in octets */
    unsigned char getModulationParameterCount() const;

    std::vector<unsigned char>& getModulationParametersList();
    const std::vector<unsigned char>& getModulationParametersList() const;
    void setModulationParametersList(const std::vector<unsigned char>&    pX);

    std::vector<unsigned char>& getAntennaPatternList();
    const std::vector<unsigned char>& getAntennaPatternList() const;
    void setAntennaPatternList(const std::vector<unsigned char>&    pX);

    std::vector<VariableTransmitterParameters>& getVariableTransmitterParametersList();
    const std::vector<VariableTransmitterParameters>& getVariableTransmitterParametersList() const;
    void setVariableTransmitterParametersList(const std::vector<VariableTransmitterParameters>&    pX);


virtual int getMarshalledSize() const;

     bool operator  ==(const TransmitterPdu& rhs) const;
};
}
