#include <dis7/TransmitterPdu.h>

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


TransmitterPdu::TransmitterPdu() : RadioCommunicationsFamilyPdu(),
   _radioEntityType(),
   _transmitState(0),
   _inputSource(0),
   _antennaLocation(),
   _relativeAntennaLocation(),
   _antennaPatternType(0),
   _frequency(0),
   _transmitFrequencyBandwidth(0.0),
   _power(0.0),
   _modulationType(),
   _cryptoSystem(0),
   _cryptoKeyId(0),
   _modulationParametersList(),
   _antennaPatternList(),
   _variableTransmitterParametersList()
{
    setPduType( 25 );
}

TransmitterPdu::~TransmitterPdu()
{
}

RadioType& TransmitterPdu::getRadioEntityType()
{
    return _radioEntityType;
}

const RadioType& TransmitterPdu::getRadioEntityType() const
{
    return _radioEntityType;
}

void TransmitterPdu::setRadioEntityType(const RadioType &pX)
{
    _radioEntityType = pX;
}

unsigned char TransmitterPdu::getTransmitState() const
{
    return _transmitState;
}

void TransmitterPdu::setTransmitState(unsigned char pX)
{
    _transmitState = pX;
}

unsigned char TransmitterPdu::getInputSource() const
{
    return _inputSource;
}

void TransmitterPdu::setInputSource(unsigned char pX)
{
    _inputSource = pX;
}

unsigned short TransmitterPdu::getVariableTransmitterParameterCount() const
{
   return checkedCount<unsigned short>(_variableTransmitterParametersList.size(),
       "TransmitterPdu: more Variable Transmitter Parameters records than the 16-bit count holds");
}

Vector3Double& TransmitterPdu::getAntennaLocation()
{
    return _antennaLocation;
}

const Vector3Double& TransmitterPdu::getAntennaLocation() const
{
    return _antennaLocation;
}

void TransmitterPdu::setAntennaLocation(const Vector3Double &pX)
{
    _antennaLocation = pX;
}

Vector3Float& TransmitterPdu::getRelativeAntennaLocation()
{
    return _relativeAntennaLocation;
}

const Vector3Float& TransmitterPdu::getRelativeAntennaLocation() const
{
    return _relativeAntennaLocation;
}

void TransmitterPdu::setRelativeAntennaLocation(const Vector3Float &pX)
{
    _relativeAntennaLocation = pX;
}

unsigned short TransmitterPdu::getAntennaPatternType() const
{
    return _antennaPatternType;
}

void TransmitterPdu::setAntennaPatternType(unsigned short pX)
{
    _antennaPatternType = pX;
}

unsigned short TransmitterPdu::getAntennaPatternCount() const
{
   return checkedCount<unsigned short>(_antennaPatternList.size(),
       "TransmitterPdu: antenna pattern exceeds the 16-bit Antenna Pattern Length");
}

unsigned long long TransmitterPdu::getFrequency() const
{
    return _frequency;
}

void TransmitterPdu::setFrequency(unsigned long long pX)
{
    _frequency = pX;
}

float TransmitterPdu::getTransmitFrequencyBandwidth() const
{
    return _transmitFrequencyBandwidth;
}

void TransmitterPdu::setTransmitFrequencyBandwidth(float pX)
{
    _transmitFrequencyBandwidth = pX;
}

float TransmitterPdu::getPower() const
{
    return _power;
}

void TransmitterPdu::setPower(float pX)
{
    _power = pX;
}

ModulationType& TransmitterPdu::getModulationType()
{
    return _modulationType;
}

const ModulationType& TransmitterPdu::getModulationType() const
{
    return _modulationType;
}

void TransmitterPdu::setModulationType(const ModulationType &pX)
{
    _modulationType = pX;
}

unsigned short TransmitterPdu::getCryptoSystem() const
{
    return _cryptoSystem;
}

void TransmitterPdu::setCryptoSystem(unsigned short pX)
{
    _cryptoSystem = pX;
}

unsigned short TransmitterPdu::getCryptoKeyId() const
{
    return _cryptoKeyId;
}

void TransmitterPdu::setCryptoKeyId(unsigned short pX)
{
    _cryptoKeyId = pX;
}

unsigned char TransmitterPdu::getModulationParameterCount() const
{
   return checkedCount<unsigned char>(_modulationParametersList.size(),
       "TransmitterPdu: modulation parameters exceed the 8-bit Length of Modulation Parameters");
}

std::vector<unsigned char>& TransmitterPdu::getModulationParametersList()
{
    return _modulationParametersList;
}

const std::vector<unsigned char>& TransmitterPdu::getModulationParametersList() const
{
    return _modulationParametersList;
}

void TransmitterPdu::setModulationParametersList(const std::vector<unsigned char>& pX)
{
     _modulationParametersList = pX;
}

std::vector<unsigned char>& TransmitterPdu::getAntennaPatternList()
{
    return _antennaPatternList;
}

const std::vector<unsigned char>& TransmitterPdu::getAntennaPatternList() const
{
    return _antennaPatternList;
}

void TransmitterPdu::setAntennaPatternList(const std::vector<unsigned char>& pX)
{
     _antennaPatternList = pX;
}

std::vector<VariableTransmitterParameters>& TransmitterPdu::getVariableTransmitterParametersList()
{
    return _variableTransmitterParametersList;
}

const std::vector<VariableTransmitterParameters>& TransmitterPdu::getVariableTransmitterParametersList() const
{
    return _variableTransmitterParametersList;
}

void TransmitterPdu::setVariableTransmitterParametersList(const std::vector<VariableTransmitterParameters>& pX)
{
     _variableTransmitterParametersList = pX;
}

void TransmitterPdu::marshal(DataStream& dataStream) const
{
    // The count getters throw for a container larger than its field; take the
    // counts before anything is written.
    const unsigned short variableTransmitterParameterCount = getVariableTransmitterParameterCount();
    const unsigned short antennaPatternCount = getAntennaPatternCount();
    const unsigned char modulationParameterCount = getModulationParameterCount();

    RadioCommunicationsFamilyPdu::marshal(dataStream); // Marshal information in superclass first
    _radioEntityType.marshal(dataStream);
    dataStream << _transmitState;
    dataStream << _inputSource;
    dataStream << variableTransmitterParameterCount;
    _antennaLocation.marshal(dataStream);
    _relativeAntennaLocation.marshal(dataStream);
    dataStream << _antennaPatternType;
    dataStream << antennaPatternCount;
    dataStream << _frequency;
    dataStream << _transmitFrequencyBandwidth;
    dataStream << _power;
    _modulationType.marshal(dataStream);
    dataStream << _cryptoSystem;
    dataStream << _cryptoKeyId;
    dataStream << modulationParameterCount;

    // 8 bits then 16 bits of unused padding
    const unsigned char padding8 = 0;
    const unsigned short padding16 = 0;
    dataStream << padding8;
    dataStream << padding16;

    for(std::size_t idx = 0; idx < _modulationParametersList.size(); idx++)
    {
        dataStream << _modulationParametersList[idx];
    }

    for(std::size_t idx = 0; idx < _antennaPatternList.size(); idx++)
    {
        dataStream << _antennaPatternList[idx];
    }

    for(std::size_t idx = 0; idx < _variableTransmitterParametersList.size(); idx++)
    {
        _variableTransmitterParametersList[idx].marshal(dataStream);
    }
}

void TransmitterPdu::unmarshal(DataStream& dataStream)
{
    RadioCommunicationsFamilyPdu::unmarshal(dataStream); // unmarshal information in superclass first
    _radioEntityType.unmarshal(dataStream);
    dataStream >> _transmitState;
    dataStream >> _inputSource;

    unsigned short variableTransmitterParameterCount = 0;
    dataStream >> variableTransmitterParameterCount;

    _antennaLocation.unmarshal(dataStream);
    _relativeAntennaLocation.unmarshal(dataStream);
    dataStream >> _antennaPatternType;

    unsigned short antennaPatternCount = 0;
    dataStream >> antennaPatternCount;

    dataStream >> _frequency;
    dataStream >> _transmitFrequencyBandwidth;
    dataStream >> _power;
    _modulationType.unmarshal(dataStream);
    dataStream >> _cryptoSystem;
    dataStream >> _cryptoKeyId;

    unsigned char modulationParameterCount = 0;
    dataStream >> modulationParameterCount;

    unsigned char padding8 = 0;
    unsigned short padding16 = 0;
    dataStream >> padding8;
    dataStream >> padding16;

    _modulationParametersList.clear();
    _modulationParametersList.resize(modulationParameterCount);
    for(std::size_t idx = 0; idx < _modulationParametersList.size(); idx++)
    {
        dataStream >> _modulationParametersList[idx];
    }

    _antennaPatternList.clear();
    _antennaPatternList.resize(antennaPatternCount);
    for(std::size_t idx = 0; idx < _antennaPatternList.size(); idx++)
    {
        dataStream >> _antennaPatternList[idx];
    }

    _variableTransmitterParametersList.clear();
    for(std::size_t idx = 0; idx < variableTransmitterParameterCount; idx++)
    {
        VariableTransmitterParameters x;
        x.unmarshal(dataStream);
        _variableTransmitterParametersList.push_back(x);
    }
}


bool TransmitterPdu::operator ==(const TransmitterPdu& rhs) const
 {
     bool ivarsEqual = true;

     ivarsEqual = RadioCommunicationsFamilyPdu::operator==(rhs);

     if( ! (_radioEntityType == rhs._radioEntityType) ) ivarsEqual = false;
     if( ! (_transmitState == rhs._transmitState) ) ivarsEqual = false;
     if( ! (_inputSource == rhs._inputSource) ) ivarsEqual = false;
     if( ! (_antennaLocation == rhs._antennaLocation) ) ivarsEqual = false;
     if( ! (_relativeAntennaLocation == rhs._relativeAntennaLocation) ) ivarsEqual = false;
     if( ! (_antennaPatternType == rhs._antennaPatternType) ) ivarsEqual = false;
     if( ! (_frequency == rhs._frequency) ) ivarsEqual = false;
     if( ! (_transmitFrequencyBandwidth == rhs._transmitFrequencyBandwidth) ) ivarsEqual = false;
     if( ! (_power == rhs._power) ) ivarsEqual = false;
     if( ! (_modulationType == rhs._modulationType) ) ivarsEqual = false;
     if( ! (_cryptoSystem == rhs._cryptoSystem) ) ivarsEqual = false;
     if( ! (_cryptoKeyId == rhs._cryptoKeyId) ) ivarsEqual = false;
     if( ! (_modulationParametersList == rhs._modulationParametersList) ) ivarsEqual = false;
     if( ! (_antennaPatternList == rhs._antennaPatternList) ) ivarsEqual = false;
     if( ! (_variableTransmitterParametersList == rhs._variableTransmitterParametersList) ) ivarsEqual = false;

    return ivarsEqual;
 }

int TransmitterPdu::getMarshalledSize() const
{
   int marshalSize = 0;

   marshalSize = RadioCommunicationsFamilyPdu::getMarshalledSize();
   marshalSize = marshalSize + _radioEntityType.getMarshalledSize();  // _radioEntityType
   marshalSize = marshalSize + 1;  // _transmitState
   marshalSize = marshalSize + 1;  // _inputSource
   marshalSize = marshalSize + 2;  // Number of Variable Transmitter Parameters records
   marshalSize = marshalSize + _antennaLocation.getMarshalledSize();  // _antennaLocation
   marshalSize = marshalSize + _relativeAntennaLocation.getMarshalledSize();  // _relativeAntennaLocation
   marshalSize = marshalSize + 2;  // _antennaPatternType
   marshalSize = marshalSize + 2;  // Antenna Pattern Length
   marshalSize = marshalSize + 8;  // _frequency
   marshalSize = marshalSize + 4;  // _transmitFrequencyBandwidth
   marshalSize = marshalSize + 4;  // _power
   marshalSize = marshalSize + _modulationType.getMarshalledSize();  // _modulationType
   marshalSize = marshalSize + 2;  // _cryptoSystem
   marshalSize = marshalSize + 2;  // _cryptoKeyId
   marshalSize = marshalSize + 1;  // Length of Modulation Parameters
   marshalSize = marshalSize + 1;  // padding, 8 bits
   marshalSize = marshalSize + 2;  // padding, 16 bits
   marshalSize = marshalSize + static_cast<int>(_modulationParametersList.size());
   marshalSize = marshalSize + static_cast<int>(_antennaPatternList.size());

   for(std::size_t idx=0; idx < _variableTransmitterParametersList.size(); idx++)
   {
        marshalSize = marshalSize + _variableTransmitterParametersList[idx].getMarshalledSize();
   }

    return marshalSize;
}
