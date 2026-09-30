// OpenDIS7 wire-format tests for the radio PDUs (IEEE 1278.1-2012 7.7.2
// Transmitter, Table 176; 7.7.3 Signal, Table 178), the Variable Transmitter
// Parameters record (6.2.95, Table 130) and the Aggregate State PDU (7.8.2,
// Table 183) with its Silent Aggregate/Entity System records (6.2.79, Table 110).
//
// The hex literals are written from the standard's tables, field by field, not
// from this library's output. All values are big-endian. Offsets are absolute
// from the start of the PDU.

#include "catch.hpp"

#include <dis7/AggregateStatePdu.h>
#include <dis7/SignalPdu.h>
#include <dis7/TransmitterPdu.h>
#include <dis7/VariableDatum.h>
#include <dis7/VariableTransmitterParameters.h>
#include <dis7/utils/DataStream.h>

#include <cstddef>
#include <stdexcept>
#include <vector>

namespace
{
typedef std::vector<unsigned char> Bytes;

template <typename T>
Bytes Marshal(const T& value)
{
  DIS::DataStream ds(DIS::BIG);
  value.marshal(ds);
  Bytes out(ds.size());
  for (std::size_t i = 0; i < out.size(); ++i)
  {
    out[i] = static_cast<unsigned char>(ds[static_cast<unsigned int>(i)]);
  }
  return out;
}

// Unmarshals `wire` into `value` and returns the read position afterwards.
template <typename T>
std::size_t Unmarshal(const Bytes& wire, T& value)
{
  DIS::DataStream ds(reinterpret_cast<const char*>(wire.data()), wire.size(), DIS::BIG);
  value.unmarshal(ds);
  return ds.GetReadPos();
}

Bytes Concat(const Bytes& a, const Bytes& b)
{
  Bytes out(a);
  out.insert(out.end(), b.begin(), b.end());
  return out;
}

unsigned int U16At(const Bytes& w, std::size_t off)
{
  return (static_cast<unsigned int>(w[off]) << 8) | w[off + 1];
}

unsigned int U32At(const Bytes& w, std::size_t off)
{
  return (static_cast<unsigned int>(w[off]) << 24) | (static_cast<unsigned int>(w[off + 1]) << 16) |
         (static_cast<unsigned int>(w[off + 2]) << 8) | w[off + 3];
}

// ---------------------------------------------------------------------------
// Transmitter PDU, IEEE 1278.1-2012 Table 176. Fixed part only (M = A = N = 0).
// The Radio Type is the AN/PRC-117 example of 6.2.71 (7.0.225.1.43.117.0).
//
//  off  size  field                                   value (hex)
//   0    1    Protocol Version                        07
//   1    1    Exercise ID                             01
//   2    1    PDU Type                                19  (25 = Transmitter)
//   3    1    Protocol Family                         04  (Radio Communications)
//   4    4    Timestamp                               00 00 00 00
//   8    2    Length                                  00 68  (104)
//  10    1    PDU Status                              00
//  11    1    Padding                                 00
//  12    2    Radio Reference ID: Site Number         02 03
//  14    2    Radio Reference ID: Application Number  04 05
//  16    2    Radio Reference ID: Reference Number    06 07
//  18    2    Radio Number                            08 09
//  20    1    Radio Type: Entity Kind                 07  (Radio)
//  21    1    Radio Type: Domain                      00  (Other)
//  22    2    Radio Type: Country                     00 E1  (225)
//  24    1    Radio Type: Category                    01
//  25    1    Radio Type: Subcategory                 2B  (43)
//  26    1    Radio Type: Specific                    75  (117)
//  27    1    Radio Type: Extra                       00
//  28    1    Transmit State                          02
//  29    1    Input Source                            01
//  30    2    Number of VTP Records (N)               00 00
//  32    8    Antenna Location X  (1.0)               3F F0 00 00 00 00 00 00
//  40    8    Antenna Location Y  (-2.0)              C0 00 00 00 00 00 00 00
//  48    8    Antenna Location Z  (0.5)               3F E0 00 00 00 00 00 00
//  56    4    Relative Antenna Location x (1.0f)      3F 80 00 00
//  60    4    Relative Antenna Location y (0.0f)      00 00 00 00
//  64    4    Relative Antenna Location z (-1.0f)     BF 80 00 00
//  68    2    Antenna Pattern Type                    00 01
//  70    2    Antenna Pattern Length (A)              00 00
//  72    8    Frequency (243000000 Hz)                00 00 00 00 0E 7B E2 C0
//  80    4    Transmit Frequency Bandwidth (25000.0f) 46 C3 50 00
//  84    4    Power (40.0f)                           42 20 00 00
//  88    2    Modulation Type: Spread Spectrum        00 00
//  90    2    Modulation Type: Major Modulation       00 01
//  92    2    Modulation Type: Detail                 00 02
//  94    2    Modulation Type: Radio System           00 01
//  96    2    Crypto System                           00 04
//  98    2    Crypto Key ID                           81 23
// 100    1    Length of Modulation Parameters (M)     00
// 101    1    Padding (8 bits)                        00
// 102    2    Padding (16 bits)                       00 00
// total 104 octets
// ---------------------------------------------------------------------------
const unsigned char kTransmitterFixedPart[] = {
  0x07, 0x01, 0x19, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00,  // PDU Header
  0x02, 0x03, 0x04, 0x05, 0x06, 0x07,                                      // Radio Reference ID
  0x08, 0x09,                                                              // Radio Number
  0x07, 0x00, 0x00, 0xE1, 0x01, 0x2B, 0x75, 0x00,                          // Radio Type
  0x02,                                                                    // Transmit State
  0x01,                                                                    // Input Source
  0x00, 0x00,                                                              // Number of VTP Records
  0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,                          // Antenna Location X
  0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,                          // Antenna Location Y
  0x3F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,                          // Antenna Location Z
  0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBF, 0x80, 0x00, 0x00,  // Relative Antenna Location
  0x00, 0x01,                                                              // Antenna Pattern Type
  0x00, 0x00,                                                              // Antenna Pattern Length
  0x00, 0x00, 0x00, 0x00, 0x0E, 0x7B, 0xE2, 0xC0,                          // Frequency
  0x46, 0xC3, 0x50, 0x00,                                                  // Transmit Frequency Bandwidth
  0x42, 0x20, 0x00, 0x00,                                                  // Power
  0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01,                          // Modulation Type
  0x00, 0x04,                                                              // Crypto System
  0x81, 0x23,                                                              // Crypto Key ID
  0x00,                                                                    // Length of Modulation Parameters
  0x00, 0x00, 0x00,                                                        // Padding (8 + 16 bits)
};

DIS::TransmitterPdu MakeTransmitter()
{
  DIS::TransmitterPdu p;
  p.setExerciseID(0x01);
  p.getEntityId().getSimulationAddress().setSite(0x0203);
  p.getEntityId().getSimulationAddress().setApplication(0x0405);
  p.getEntityId().setEntityNumber(0x0607);
  p.setRadioId(0x0809);
  p.getRadioEntityType().setEntityKind(7);
  p.getRadioEntityType().setDomain(0);
  p.getRadioEntityType().setCountry(225);
  p.getRadioEntityType().setCategory(1);
  p.getRadioEntityType().setSubcategory(43);
  p.getRadioEntityType().setSpecific(117);
  p.getRadioEntityType().setExtra(0);
  p.setTransmitState(2);
  p.setInputSource(1);
  p.getAntennaLocation().setX(1.0);
  p.getAntennaLocation().setY(-2.0);
  p.getAntennaLocation().setZ(0.5);
  p.getRelativeAntennaLocation().setX(1.0f);
  p.getRelativeAntennaLocation().setY(0.0f);
  p.getRelativeAntennaLocation().setZ(-1.0f);
  p.setAntennaPatternType(1);
  p.setFrequency(243000000ULL);
  p.setTransmitFrequencyBandwidth(25000.0f);
  p.setPower(40.0f);
  p.getModulationType().setSpreadSpectrum(0);
  p.getModulationType().setMajorModulation(1);
  p.getModulationType().setDetail(2);
  p.getModulationType().setRadioSystem(1);
  p.setCryptoSystem(4);
  p.setCryptoKeyId(0x8123);
  p.setLength(static_cast<unsigned short>(p.getMarshalledSize()));
  return p;
}

// ---------------------------------------------------------------------------
// Signal PDU, IEEE 1278.1-2012 Table 178, with one data octet: a 256-bit
// fixed part, K = 8 data bits and P = 24 padding bits.
//
//  off  size  field                                   value (hex)
//   0    1    Protocol Version                        07
//   1    1    Exercise ID                             01
//   2    1    PDU Type                                1A  (26 = Signal)
//   3    1    Protocol Family                         04  (Radio Communications)
//   4    4    Timestamp                               00 00 00 00
//   8    2    Length                                  00 24  (36)
//  10    1    PDU Status                              00
//  11    1    Padding                                 00
//  12    2    Radio Reference ID: Site Number         02 03
//  14    2    Radio Reference ID: Application Number  04 05
//  16    2    Radio Reference ID: Reference Number    06 07
//  18    2    Radio Number                            08 09
//  20    2    Encoding Scheme                         0C 0D
//  22    2    TDL Type                                0E 0F
//  24    4    Sample Rate                             10 11 12 13
//  28    2    Data Length (K, bits)                   00 08
//  30    2    Samples                                 14 15
//  32    1    Data                                    AB
//  33    3    Padding to 32-bit boundary              00 00 00
// total 36 octets
// ---------------------------------------------------------------------------
const unsigned char kSignalOneOctet[] = {
  0x07, 0x01, 0x1A, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00,  // PDU Header
  0x02, 0x03, 0x04, 0x05, 0x06, 0x07,                                      // Radio Reference ID
  0x08, 0x09,                                                              // Radio Number
  0x0C, 0x0D,                                                              // Encoding Scheme
  0x0E, 0x0F,                                                              // TDL Type
  0x10, 0x11, 0x12, 0x13,                                                  // Sample Rate
  0x00, 0x08,                                                              // Data Length
  0x14, 0x15,                                                              // Samples
  0xAB,                                                                    // Data
  0x00, 0x00, 0x00,                                                        // Padding
};

DIS::SignalPdu MakeSignal()
{
  DIS::SignalPdu p;
  p.setExerciseID(0x01);
  p.getEntityId().getSimulationAddress().setSite(0x0203);
  p.getEntityId().getSimulationAddress().setApplication(0x0405);
  p.getEntityId().setEntityNumber(0x0607);
  p.setRadioId(0x0809);
  p.setEncodingScheme(0x0C0D);
  p.setTdlType(0x0E0F);
  p.setSampleRate(0x10111213u);
  p.setSamples(0x1415);
  p.setData(std::vector<uint8_t>(1, 0xAB));
  p.setLength(static_cast<unsigned short>(p.getMarshalledSize()));
  return p;
}

// A Signal PDU carrying Data Length `k` (bits) with `data`, padded per Table 178.
Bytes SignalWireWithK(unsigned int k, const Bytes& data)
{
  const std::size_t padding = (4 - data.size() % 4) % 4;
  const std::size_t length = 32 + data.size() + padding;
  const unsigned char head[] = {
    0x07, 0x01, 0x1A, 0x04, 0x00, 0x00, 0x00, 0x00,                          // header
    static_cast<unsigned char>(length >> 8), static_cast<unsigned char>(length & 0xFF), 0x00, 0x00,
    0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04,                          // Radio Reference ID, Radio Number
    0x00, 0x05, 0x00, 0x06, 0x00, 0x00, 0x1F, 0x40,                          // Encoding Scheme, TDL Type, Sample Rate
    static_cast<unsigned char>(k >> 8), static_cast<unsigned char>(k & 0xFF),  // Data Length
    0x00, 0x01,                                                              // Samples
  };
  Bytes w(head, head + sizeof(head));
  w.insert(w.end(), data.begin(), data.end());
  // The unused low-order bits of a partial last octet are padding and zero.
  if (k % 8 != 0 && !data.empty())
  {
    w.back() = static_cast<unsigned char>(w.back() & (0xFF << (8 - k % 8)));
  }
  w.resize(w.size() + padding, 0x00);
  return w;
}

// ---------------------------------------------------------------------------
// Aggregate State PDU, IEEE 1278.1-2012 Table 183, with W = 1, J = 0, N = 1,
// S = 1 (one appearance record) and M = 1 (a 32-bit variable datum).
//
//  off  size  field                                   value (hex)
//   0    1    Protocol Version                        07
//   1    1    Exercise ID                             01
//   2    1    PDU Type                                21  (33 = Aggregate State)
//   3    1    Protocol Family                         07  (Entity Management)
//   4    4    Timestamp                               00 00 00 00
//   8    2    Length                                  00 BC  (188)
//  10    1    PDU Status                              00
//  11    1    Padding                                 00
//  12    6    Aggregate ID: Site, Application, Aggregate  00 01 00 02 00 03
//  18    1    Force ID                                01
//  19    1    Aggregate State                         01
//  20    8    Aggregate Type: Kind, Domain, Country, Category, Subcategory, Specific, Extra
//                                                     01 01 00 E1 03 04 05 06
//  28    4    Formation                               00 00 00 02
//  32    1    Aggregate Marking: Character Set        01
//  33   31    Aggregate Marking: characters           "ALPHA" 41 4C 50 48 41, then 26 x 00
//  64    4    Dimensions x (100.0f)                   42 C8 00 00
//  68    4    Dimensions y (50.0f)                    42 48 00 00
//  72    4    Dimensions z (10.0f)                    41 20 00 00
//  76    4    Orientation Psi (1.0f)                  3F 80 00 00
//  80    4    Orientation Theta (0.0f)                00 00 00 00
//  84    4    Orientation Phi (0.5f)                  3F 00 00 00
//  88    8    Center of Mass X (1000.0)               40 8F 40 00 00 00 00 00
//  96    8    Center of Mass Y (-2000.0)              C0 9F 40 00 00 00 00 00
// 104    8    Center of Mass Z (3000.0)               40 A7 70 00 00 00 00 00
// 112    4    Velocity x (1.0f)                       3F 80 00 00
// 116    4    Velocity y (2.0f)                       40 00 00 00
// 120    4    Velocity z (-1.0f)                      BF 80 00 00
// 124    2    Number of Aggregate IDs (W)             00 01
// 126    2    Number of Entity IDs (J)                00 00
// 128    2    Number of Silent Aggregate Systems (N)  00 01
// 130    2    Number of Silent Entity Systems (S)     00 01
// 132    6    Aggregate ID #1                         00 01 00 02 00 04
// 138    2    Padding to 32-bit boundary (W+J odd)    00 00
// 140    2    Silent Aggregate #1: Number of Aggregates  00 05
// 142    2    Silent Aggregate #1: Padding            00 00
// 144    8    Silent Aggregate #1: Aggregate Type     01 01 00 E1 03 04 05 07
// 152    2    Silent Entity #1: Number of Entities    00 0A
// 154    2    Silent Entity #1: Number of Appearance Records (A)  00 01
// 156    8    Silent Entity #1: Entity Type           01 01 00 E1 01 01 03 00
// 164    4    Silent Entity #1: Entity Appearance #1  00 00 00 01
// 168    4    Number of Variable Datum Records (M)    00 00 00 01
// 172    4    Variable Datum #1: ID                   00 00 01 00
// 176    4    Variable Datum #1: Length (K, bits)     00 00 00 20
// 180    4    Variable Datum #1: Value                DE AD BE EF
// 184    4    Variable Datum #1: Padding to 64 bits   00 00 00 00
// total 188 octets
// ---------------------------------------------------------------------------
const unsigned char kAggregateState[] = {
  0x07, 0x01, 0x21, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBC, 0x00, 0x00,  // PDU Header
  0x00, 0x01, 0x00, 0x02, 0x00, 0x03,                                      // Aggregate ID
  0x01,                                                                    // Force ID
  0x01,                                                                    // Aggregate State
  0x01, 0x01, 0x00, 0xE1, 0x03, 0x04, 0x05, 0x06,                          // Aggregate Type
  0x00, 0x00, 0x00, 0x02,                                                  // Formation
  0x01, 0x41, 0x4C, 0x50, 0x48, 0x41, 0x00, 0x00,                          // Aggregate Marking
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x42, 0xC8, 0x00, 0x00, 0x42, 0x48, 0x00, 0x00, 0x41, 0x20, 0x00, 0x00,  // Dimensions
  0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00,  // Orientation
  0x40, 0x8F, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,                          // Center of Mass X
  0xC0, 0x9F, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,                          // Center of Mass Y
  0x40, 0xA7, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00,                          // Center of Mass Z
  0x3F, 0x80, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0xBF, 0x80, 0x00, 0x00,  // Velocity
  0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01,                          // W, J, N, S
  0x00, 0x01, 0x00, 0x02, 0x00, 0x04,                                      // Aggregate ID #1
  0x00, 0x00,                                                              // Padding
  0x00, 0x05, 0x00, 0x00, 0x01, 0x01, 0x00, 0xE1, 0x03, 0x04, 0x05, 0x07,  // Silent Aggregate #1
  0x00, 0x0A, 0x00, 0x01, 0x01, 0x01, 0x00, 0xE1, 0x01, 0x01, 0x03, 0x00,  // Silent Entity #1
  0x00, 0x00, 0x00, 0x01,                                                  //   Entity Appearance #1
  0x00, 0x00, 0x00, 0x01,                                                  // M
  0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x20,                          // Variable Datum #1 ID, Length
  0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x00, 0x00, 0x00,                          //   Value, Padding
};

const std::size_t kAggregateFixedOctets = 132;
const std::size_t kAggregateCountsOffset = 124;  // W at 124, J 126, N 128, S 130

DIS::AggregateIdentifier MakeAggregateId(unsigned short n)
{
  DIS::AggregateIdentifier id;
  id.getSimulationAddress().setSite(1);
  id.getSimulationAddress().setApplication(2);
  id.setAggregateID(n);
  return id;
}

DIS::EntityID MakeEntityId(unsigned short n)
{
  DIS::EntityID id;
  id.getSimulationAddress().setSite(1);
  id.getSimulationAddress().setApplication(2);
  id.setEntityNumber(n);
  return id;
}

DIS::VariableDatum MakeDatum(unsigned int id, const char* value, unsigned int octets)
{
  DIS::VariableDatum d;
  d.setVariableDatumID(id);
  d.setVariableDatums(value, octets);
  return d;
}

DIS::AggregateStatePdu MakeAggregateState()
{
  DIS::AggregateStatePdu p;
  p.setExerciseID(0x01);
  p.setAggregateID(MakeAggregateId(3));
  p.setForceID(1);
  p.setAggregateState(1);
  p.getAggregateType().setAggregateKind(1);
  p.getAggregateType().setDomain(1);
  p.getAggregateType().setCountry(225);
  p.getAggregateType().setCategory(3);
  p.getAggregateType().setSubcategory(4);
  p.getAggregateType().setSpecific(5);
  p.getAggregateType().setExtra(6);
  p.setFormation(2);
  p.getAggregateMarking().setCharacterSet(1);
  unsigned char marking[31] = {'A', 'L', 'P', 'H', 'A'};
  p.getAggregateMarking().setCharacters(marking);
  p.getDimensions().setX(100.0f);
  p.getDimensions().setY(50.0f);
  p.getDimensions().setZ(10.0f);
  p.getOrientation().setPsi(1.0f);
  p.getOrientation().setTheta(0.0f);
  p.getOrientation().setPhi(0.5f);
  p.getCenterOfMass().setX(1000.0);
  p.getCenterOfMass().setY(-2000.0);
  p.getCenterOfMass().setZ(3000.0);
  p.getVelocity().setX(1.0f);
  p.getVelocity().setY(2.0f);
  p.getVelocity().setZ(-1.0f);

  p.getAggregateIDList().push_back(MakeAggregateId(4));

  DIS::SilentAggregateSystem silentAggregate;
  silentAggregate.setNumberOfAggregates(5);
  silentAggregate.getAggregateType() = p.getAggregateType();
  silentAggregate.getAggregateType().setExtra(7);
  p.getSilentAggregateSystemList().push_back(silentAggregate);

  DIS::SilentEntitySystem silentEntity;
  silentEntity.setNumberOfEntities(10);
  silentEntity.getEntityType().setEntityKind(1);
  silentEntity.getEntityType().setDomain(1);
  silentEntity.getEntityType().setCountry(225);
  silentEntity.getEntityType().setCategory(1);
  silentEntity.getEntityType().setSubcategory(1);
  silentEntity.getEntityType().setSpecific(3);
  silentEntity.getEntityType().setExtra(0);
  silentEntity.getAppearanceRecordList().push_back(1);
  p.getSilentEntitySystemList().push_back(silentEntity);

  const char value[] = {static_cast<char>(0xDE), static_cast<char>(0xAD), static_cast<char>(0xBE), static_cast<char>(0xEF)};
  p.getVariableDatumList().push_back(MakeDatum(0x100, value, 4));

  p.setLength(static_cast<unsigned short>(p.getMarshalledSize()));
  return p;
}
}  // namespace

// ===========================================================================
// Variable Transmitter Parameters record (6.2.95, Table 130)
// ===========================================================================

TEST_CASE("dis7 VariableTransmitterParameters matches Table 130", "[dis7][VTP]")
{
  // Record Type 00 00 00 01, Record Length 00 10 (16 = 6 + K 10 + P 0),
  // record-specific fields 01..0A, no padding needed.
  const unsigned char expected[] = {
    0x00, 0x00, 0x00, 0x01,                                      // Record Type
    0x00, 0x10,                                                  // Record Length
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A,  // Record-Specific fields
  };
  DIS::VariableTransmitterParameters vtp;
  vtp.setRecordType(1);
  vtp.setRecordSpecificFields(Bytes(expected + 6, expected + sizeof(expected)));

  const Bytes wire = Marshal(vtp);
  REQUIRE(wire == Bytes(expected, expected + sizeof(expected)));
  REQUIRE(vtp.getRecordLength() == 16);
  REQUIRE(vtp.getMarshalledSize() == 16);

  DIS::VariableTransmitterParameters copy;
  REQUIRE(Unmarshal(wire, copy) == wire.size());
  REQUIRE(copy == vtp);
  REQUIRE(Marshal(copy) == wire);
}

TEST_CASE("dis7 VariableTransmitterParameters pads to a 64-bit boundary", "[dis7][VTP]")
{
  DIS::VariableTransmitterParameters vtp;
  vtp.setRecordType(3);
  const unsigned char fields[] = {0xC1, 0xC2, 0xC3};
  vtp.setRecordSpecificFields(Bytes(fields, fields + 3));

  // Record Length 16 = 6 + K 3 + P 7.
  const unsigned char expected[] = {
    0x00, 0x00, 0x00, 0x03,                          // Record Type
    0x00, 0x10,                                      // Record Length
    0xC1, 0xC2, 0xC3,                                // Record-Specific fields
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        // Padding to 64-bit boundary
  };
  const Bytes wire = Marshal(vtp);
  REQUIRE(wire == Bytes(expected, expected + sizeof(expected)));
  REQUIRE(vtp.getMarshalledSize() == 16);

  // unmarshal keeps Record Length - 6 octets, the padding included.
  DIS::VariableTransmitterParameters copy;
  REQUIRE(Unmarshal(wire, copy) == wire.size());
  REQUIRE(copy.getRecordSpecificFields().size() == 10);
  REQUIRE(Marshal(copy) == wire);

  // An empty record is the 6-octet header plus 2 padding octets.
  DIS::VariableTransmitterParameters empty;
  const unsigned char emptyExpected[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00};
  REQUIRE(Marshal(empty) == Bytes(emptyExpected, emptyExpected + sizeof(emptyExpected)));
}

TEST_CASE("dis7 VariableTransmitterParameters rejects lengths the record cannot carry", "[dis7][VTP]")
{
  // Largest record: 65528 octets (the largest multiple of 8 below 65536).
  DIS::VariableTransmitterParameters vtp;
  vtp.getRecordSpecificFields().resize(65522);
  REQUIRE(vtp.getRecordLength() == 65528);

  vtp.getRecordSpecificFields().resize(65523);  // would pad to 65536
  DIS::DataStream ds(DIS::BIG);
  REQUIRE_THROWS_AS(vtp.marshal(ds), std::length_error);
  REQUIRE(ds.size() == 0);

  // A Record Length shorter than the record header is malformed.
  const unsigned char shortLength[] = {0x00, 0x00, 0x00, 0x01, 0x00, 0x05, 0x00, 0x00};
  DIS::VariableTransmitterParameters copy;
  REQUIRE_THROWS_AS(Unmarshal(Bytes(shortLength, shortLength + sizeof(shortLength)), copy), std::length_error);
}

// ===========================================================================
// Transmitter PDU (7.7.2, Table 176)
// ===========================================================================

TEST_CASE("dis7 TransmitterPdu fixed part matches Table 176", "[dis7][TransmitterPdu]")
{
  const Bytes expected(kTransmitterFixedPart, kTransmitterFixedPart + sizeof(kTransmitterFixedPart));
  REQUIRE(expected.size() == 104);

  const DIS::TransmitterPdu fixture = MakeTransmitter();
  REQUIRE(fixture.getMarshalledSize() == 104);
  REQUIRE(Marshal(fixture) == expected);

  DIS::TransmitterPdu got;
  REQUIRE(Unmarshal(expected, got) == expected.size());
  REQUIRE(got.getProtocolVersion() == 7);
  REQUIRE(got.getPduType() == 25);
  REQUIRE(got.getProtocolFamily() == 4);
  REQUIRE(got.getLength() == 104);
  REQUIRE(got.getEntityId().getSimulationAddress().getSite() == 0x0203);
  REQUIRE(got.getEntityId().getSimulationAddress().getApplication() == 0x0405);
  REQUIRE(got.getEntityId().getEntityNumber() == 0x0607);
  REQUIRE(got.getRadioId() == 0x0809);
  REQUIRE(got.getRadioEntityType().getEntityKind() == 7);
  REQUIRE(got.getRadioEntityType().getCountry() == 225);
  REQUIRE(got.getRadioEntityType().getSubcategory() == 43);
  REQUIRE(got.getRadioEntityType().getSpecific() == 117);
  REQUIRE(got.getTransmitState() == 2);
  REQUIRE(got.getInputSource() == 1);
  REQUIRE(got.getAntennaLocation().getY() == -2.0);
  REQUIRE(got.getRelativeAntennaLocation().getZ() == -1.0f);
  REQUIRE(got.getAntennaPatternType() == 1);
  REQUIRE(got.getFrequency() == 243000000ULL);
  REQUIRE(got.getTransmitFrequencyBandwidth() == 25000.0f);
  REQUIRE(got.getPower() == 40.0f);
  REQUIRE(got.getModulationType().getDetail() == 2);
  REQUIRE(got.getCryptoSystem() == 4);
  REQUIRE(got.getCryptoKeyId() == 0x8123);
  REQUIRE(got.getModulationParameterCount() == 0);
  REQUIRE(got.getAntennaPatternCount() == 0);
  REQUIRE(got.getVariableTransmitterParameterCount() == 0);
  REQUIRE(got == fixture);
  REQUIRE(Marshal(got) == expected);
}

TEST_CASE("dis7 TransmitterPdu parameter records section follows Table 176", "[dis7][TransmitterPdu]")
{
  DIS::TransmitterPdu p = MakeTransmitter();
  const unsigned char modulation[] = {0xA1, 0xA2, 0xA3};
  const unsigned char antenna[] = {0xB1, 0xB2};
  p.setModulationParametersList(Bytes(modulation, modulation + 3));
  p.setAntennaPatternList(Bytes(antenna, antenna + 2));

  // VTP #1: K = 3 record-specific octets, stored with their P = 7 padding
  // octets so the record compares equal after a round trip.
  DIS::VariableTransmitterParameters vtp1;
  vtp1.setRecordType(3);
  const unsigned char fields1[] = {0xC1, 0xC2, 0xC3, 0, 0, 0, 0, 0, 0, 0};
  vtp1.setRecordSpecificFields(Bytes(fields1, fields1 + sizeof(fields1)));
  // VTP #2: K = 2, P = 0.
  DIS::VariableTransmitterParameters vtp2;
  vtp2.setRecordType(4);
  const unsigned char fields2[] = {0xD1, 0xD2};
  vtp2.setRecordSpecificFields(Bytes(fields2, fields2 + 2));
  p.getVariableTransmitterParametersList().push_back(vtp1);
  p.getVariableTransmitterParametersList().push_back(vtp2);
  p.setLength(static_cast<unsigned short>(p.getMarshalledSize()));

  //  off  size  field                                   value (hex)
  // 104    3    Modulation Parameters (M = 3)           A1 A2 A3
  // 107    2    Antenna Pattern (A = 2)                 B1 B2
  // 109    4    VTP #1 Record Type                      00 00 00 03
  // 113    2    VTP #1 Record Length (6 + 3 + 7)        00 10
  // 115    3    VTP #1 Record-Specific fields           C1 C2 C3
  // 118    7    VTP #1 Padding to 64-bit boundary       00 x 7
  // 125    4    VTP #2 Record Type                      00 00 00 04
  // 129    2    VTP #2 Record Length (6 + 2 + 0)        00 08
  // 131    2    VTP #2 Record-Specific fields           D1 D2
  // total 133 octets
  const unsigned char expectedTail[] = {
    0xA1, 0xA2, 0xA3,                                                  // Modulation Parameters
    0xB1, 0xB2,                                                        // Antenna Pattern
    0x00, 0x00, 0x00, 0x03, 0x00, 0x10, 0xC1, 0xC2, 0xC3,              // VTP #1
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,                          //   padding
    0x00, 0x00, 0x00, 0x04, 0x00, 0x08, 0xD1, 0xD2,                    // VTP #2
  };

  const Bytes wire = Marshal(p);
  REQUIRE(wire.size() == 133);
  REQUIRE(p.getMarshalledSize() == 133);
  REQUIRE(U16At(wire, 8) == 133);    // Length
  REQUIRE(U16At(wire, 30) == 2);     // Number of VTP Records
  REQUIRE(U16At(wire, 70) == 2);     // Antenna Pattern Length (octets)
  REQUIRE(wire[100] == 3);           // Length of Modulation Parameters
  REQUIRE(wire[101] == 0);           // Padding
  REQUIRE(U16At(wire, 102) == 0);    // Padding
  REQUIRE(Bytes(wire.begin() + 104, wire.end()) == Bytes(expectedTail, expectedTail + sizeof(expectedTail)));

  DIS::TransmitterPdu got;
  REQUIRE(Unmarshal(wire, got) == wire.size());
  REQUIRE(got.getModulationParametersList() == Bytes(modulation, modulation + 3));
  REQUIRE(got.getAntennaPatternList() == Bytes(antenna, antenna + 2));
  REQUIRE(got.getVariableTransmitterParametersList().size() == 2);
  REQUIRE(got.getVariableTransmitterParametersList()[1].getRecordType() == 4);
  REQUIRE(got == p);
  REQUIRE(Marshal(got) == wire);

  // The record reads exactly its own octets when more data follows it.
  Bytes longer = wire;
  longer.push_back(0xEE);
  DIS::TransmitterPdu got2;
  REQUIRE(Unmarshal(longer, got2) == wire.size());
}

TEST_CASE("dis7 TransmitterPdu refuses containers larger than their count fields", "[dis7][TransmitterPdu]")
{
  {
    DIS::TransmitterPdu p;
    p.getModulationParametersList().resize(256);
    DIS::DataStream ds(DIS::BIG);
    REQUIRE_THROWS_AS(p.marshal(ds), std::length_error);
    REQUIRE(ds.size() == 0);
    p.getModulationParametersList().resize(255);
    REQUIRE_NOTHROW(p.marshal(ds));
  }
  {
    DIS::TransmitterPdu p;
    p.getAntennaPatternList().resize(65536);
    DIS::DataStream ds(DIS::BIG);
    REQUIRE_THROWS_AS(p.marshal(ds), std::length_error);
    REQUIRE(ds.size() == 0);
  }
  {
    DIS::TransmitterPdu p;
    p.getVariableTransmitterParametersList().resize(65536);
    DIS::DataStream ds(DIS::BIG);
    REQUIRE_THROWS_AS(p.marshal(ds), std::length_error);
    REQUIRE(ds.size() == 0);
  }
}

// ===========================================================================
// Signal PDU (7.7.3, Table 178)
// ===========================================================================

TEST_CASE("dis7 SignalPdu matches Table 178", "[dis7][SignalPdu]")
{
  const Bytes expected(kSignalOneOctet, kSignalOneOctet + sizeof(kSignalOneOctet));
  REQUIRE(expected.size() == 36);

  const DIS::SignalPdu fixture = MakeSignal();
  REQUIRE(fixture.getMarshalledSize() == 36);
  REQUIRE(Marshal(fixture) == expected);

  DIS::SignalPdu got;
  REQUIRE(Unmarshal(expected, got) == expected.size());  // padding consumed exactly
  REQUIRE(got.getProtocolVersion() == 7);
  REQUIRE(got.getPduType() == 26);
  REQUIRE(got.getProtocolFamily() == 4);
  REQUIRE(got.getEntityId().getSimulationAddress().getSite() == 0x0203);
  REQUIRE(got.getEntityId().getEntityNumber() == 0x0607);
  REQUIRE(got.getRadioId() == 0x0809);
  REQUIRE(got.getEncodingScheme() == 0x0C0D);
  REQUIRE(got.getTdlType() == 0x0E0F);
  REQUIRE(got.getSampleRate() == 0x10111213u);
  REQUIRE(got.getDataLength() == 8);
  REQUIRE(got.getSamples() == 0x1415);
  REQUIRE(got.getData() == std::vector<uint8_t>(1, 0xAB));
  REQUIRE(got == fixture);
}

TEST_CASE("dis7 SignalPdu pads whole-octet data to a 32-bit boundary", "[dis7][SignalPdu]")
{
  const unsigned char data[] = {0x11, 0x22, 0x33};  // K = 24 bits, P = 8 bits
  DIS::SignalPdu p;
  p.setData(std::vector<uint8_t>(data, data + 3));
  p.setSamples(9);

  const Bytes wire = Marshal(p);
  REQUIRE(wire.size() == 36);
  REQUIRE(U16At(wire, 28) == 24);
  REQUIRE(wire[35] == 0x00);

  DIS::SignalPdu got;
  REQUIRE(Unmarshal(wire, got) == wire.size());
  REQUIRE(got.getDataLength() == 24);
  REQUIRE(got == p);
}

TEST_CASE("dis7 SignalPdu Data Length is in bits and round-trips a partial last octet", "[dis7][SignalPdu]")
{
  struct Case { unsigned int k; std::size_t octets; std::size_t padding; };
  const Case cases[] = {{1, 1, 3}, {9, 2, 2}, {33, 5, 3}, {65535, 8192, 0}};
  for (std::size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i)
  {
    const Case& c = cases[i];
    INFO("K = " << c.k);
    const Bytes data(c.octets, 0x5A);
    const Bytes wire = SignalWireWithK(c.k, data);
    REQUIRE(wire.size() == 32 + c.octets + c.padding);

    DIS::SignalPdu got;
    REQUIRE(Unmarshal(wire, got) == wire.size());
    REQUIRE(got.getDataLength() == c.k);
    REQUIRE(got.getData().size() == c.octets);
    REQUIRE(got.getMarshalledSize() == static_cast<int>(wire.size()));
    REQUIRE(Marshal(got) == wire);  // bit-exact, K included
  }
}

TEST_CASE("dis7 SignalPdu setDataLength sets a partial-octet K for the current data", "[dis7][SignalPdu]")
{
  DIS::SignalPdu p;
  p.setData(std::vector<uint8_t>(2, 0xFF));
  REQUIRE(p.getDataLength() == 16);
  p.setDataLength(9);
  REQUIRE(p.getDataLength() == 9);
  REQUIRE(U16At(Marshal(p), 28) == 9);
  REQUIRE_THROWS_AS(p.setDataLength(17), std::length_error);  // needs 3 octets
  REQUIRE_THROWS_AS(p.setDataLength(8), std::length_error);   // needs 1 octet
  REQUIRE(p.getDataLength() == 9);

  // Replacing the data makes K the whole-octet count again.
  p.getData().push_back(0x01);
  REQUIRE(p.getDataLength() == 24);
}

TEST_CASE("dis7 SignalPdu rejects data beyond the 16-bit Data Length", "[dis7][SignalPdu]")
{
  DIS::SignalPdu p;
  REQUIRE_THROWS_AS(p.setData(std::vector<uint8_t>(8192)), std::length_error);

  p.setData(std::vector<uint8_t>(8191));
  REQUIRE(p.getDataLength() == 65528);

  p.getData().resize(8192);  // bypasses setData
  DIS::DataStream ds(DIS::BIG);
  REQUIRE_THROWS_AS(p.marshal(ds), std::length_error);
  REQUIRE(ds.size() == 0);
  REQUIRE_THROWS_AS(p.getDataLength(), std::length_error);

  bool equal = false;
  REQUIRE_NOTHROW(equal = (p == p));  // comparison never throws
  REQUIRE(equal);
  DIS::SignalPdu other(p);
  other.getData()[0] = 0x01;
  REQUIRE_NOTHROW(equal = (p == other));
  REQUIRE_FALSE(equal);
}

// ===========================================================================
// Aggregate State PDU (7.8.2, Table 183)
// ===========================================================================

TEST_CASE("dis7 AggregateStatePdu matches Table 183", "[dis7][AggregateStatePdu]")
{
  const Bytes expected(kAggregateState, kAggregateState + sizeof(kAggregateState));
  REQUIRE(expected.size() == 188);

  const DIS::AggregateStatePdu fixture = MakeAggregateState();
  REQUIRE(fixture.getMarshalledSize() == 188);
  REQUIRE(Marshal(fixture) == expected);

  DIS::AggregateStatePdu got;
  REQUIRE(Unmarshal(expected, got) == expected.size());
  REQUIRE(got.getPduType() == 33);
  REQUIRE(got.getProtocolFamily() == 7);
  REQUIRE(got.getAggregateID().getAggregateID() == 3);
  REQUIRE(got.getForceID() == 1);
  REQUIRE(got.getAggregateType().getCountry() == 225);
  REQUIRE(got.getFormation() == 2);
  REQUIRE(got.getAggregateMarking().getCharacters()[4] == 'A');
  REQUIRE(got.getOrientation().getPhi() == 0.5f);
  REQUIRE(got.getCenterOfMass().getY() == -2000.0);
  REQUIRE(got.getNumberOfDisAggregates() == 1);
  REQUIRE(got.getNumberOfDisEntities() == 0);
  REQUIRE(got.getNumberOfSilentAggregateTypes() == 1);
  REQUIRE(got.getNumberOfSilentEntityTypes() == 1);
  REQUIRE(got.getAggregateIDList()[0].getAggregateID() == 4);
  REQUIRE(got.getSilentAggregateSystemList()[0].getNumberOfAggregates() == 5);
  REQUIRE(got.getSilentAggregateSystemList()[0].getAggregateType().getExtra() == 7);
  REQUIRE(got.getSilentEntitySystemList()[0].getNumberOfEntities() == 10);
  REQUIRE(got.getSilentEntitySystemList()[0].getNumberOfAppearanceRecords() == 1);
  REQUIRE(got.getSilentEntitySystemList()[0].getAppearanceRecordList()[0] == 1u);
  REQUIRE(got.getNumberOfVariableDatumRecords() == 1);
  REQUIRE(got.getVariableDatumList()[0].getVariableDatumID() == 0x100u);
  REQUIRE(got.getVariableDatumList()[0].getVariableDatumLength() == 32u);
  REQUIRE(got == fixture);
  REQUIRE(Marshal(got) == expected);
}

TEST_CASE("dis7 AggregateStatePdu pads the ID lists to a 32-bit boundary", "[dis7][AggregateStatePdu]")
{
  // W + J odd -> 2 padding octets; even -> none.
  struct Case { std::size_t w; std::size_t j; std::size_t padding; };
  const Case cases[] = {{0, 0, 0}, {1, 0, 2}, {0, 1, 2}, {1, 1, 0}, {0, 2, 0}, {2, 0, 0}, {1, 2, 2}, {2, 1, 2}, {2, 2, 0}, {3, 4, 2}};
  for (std::size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i)
  {
    const Case& c = cases[i];
    INFO("W = " << c.w << ", J = " << c.j);
    DIS::AggregateStatePdu pdu;
    for (std::size_t a = 0; a < c.w; ++a) pdu.getAggregateIDList().push_back(MakeAggregateId(static_cast<unsigned short>(200 + a)));
    for (std::size_t e = 0; e < c.j; ++e) pdu.getEntityIDList().push_back(MakeEntityId(static_cast<unsigned short>(10 + e)));
    // A silent aggregate system after the padding shows where the padding ends.
    DIS::SilentAggregateSystem silent;
    silent.setNumberOfAggregates(0xABCD);
    pdu.getSilentAggregateSystemList().push_back(silent);

    const Bytes wire = Marshal(pdu);
    const std::size_t listsEnd = kAggregateFixedOctets + 6 * (c.w + c.j);
    REQUIRE(wire.size() == listsEnd + c.padding + 12 + 4);
    REQUIRE(wire.size() % 4 == 0);
    REQUIRE(pdu.getMarshalledSize() == static_cast<int>(wire.size()));
    REQUIRE(U16At(wire, kAggregateCountsOffset) == c.w);
    REQUIRE(U16At(wire, kAggregateCountsOffset + 2) == c.j);
    REQUIRE(U16At(wire, kAggregateCountsOffset + 4) == 1);  // N
    REQUIRE(U16At(wire, kAggregateCountsOffset + 6) == 0);  // S
    if (c.w > 0) REQUIRE(U16At(wire, kAggregateFixedOctets + 4) == 200);  // Aggregate IDs come first
    for (std::size_t p = listsEnd; p < listsEnd + c.padding; ++p) REQUIRE(wire[p] == 0);
    REQUIRE(U16At(wire, listsEnd + c.padding) == 0xABCD);
    REQUIRE(U32At(wire, wire.size() - 4) == 0);  // M

    DIS::AggregateStatePdu got;
    REQUIRE(Unmarshal(wire, got) == wire.size());
    REQUIRE(got == pdu);
    REQUIRE(Marshal(got) == wire);
  }
}

TEST_CASE("dis7 AggregateStatePdu round-trips silent systems and variable datums", "[dis7][AggregateStatePdu]")
{
  DIS::AggregateStatePdu pdu = MakeAggregateState();
  pdu.getEntityIDList().push_back(MakeEntityId(10));  // W + J = 2, no padding

  DIS::SilentEntitySystem noAppearances;
  noAppearances.setNumberOfEntities(3);
  noAppearances.getEntityType().setEntityKind(1);
  DIS::SilentEntitySystem twoAppearances;
  twoAppearances.setNumberOfEntities(2);
  twoAppearances.getAppearanceRecordList().push_back(0x00010000u);
  twoAppearances.getAppearanceRecordList().push_back(0x00800000u);
  pdu.getSilentEntitySystemList().push_back(noAppearances);
  pdu.getSilentEntitySystemList().push_back(twoAppearances);

  DIS::SilentAggregateSystem secondAggregate;
  secondAggregate.setNumberOfAggregates(2);
  pdu.getSilentAggregateSystemList().push_back(secondAggregate);

  const char twelve[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};  // 96 bits, padded to 128
  const char eight[] = {8, 7, 6, 5, 4, 3, 2, 1};                   // 64 bits, no padding
  pdu.getVariableDatumList().push_back(MakeDatum(0x200, twelve, 12));
  pdu.getVariableDatumList().push_back(MakeDatum(0x300, eight, 8));
  pdu.getVariableDatumList().push_back(DIS::VariableDatum());       // 0 bits

  const Bytes wire = Marshal(pdu);
  // 132 fixed + 12 IDs + 0 padding + 2 x 12 silent aggregates
  // + (16 + 12 + 20) silent entities + 4 M + (16 + 24 + 16 + 8) variable datums
  REQUIRE(wire.size() == 132 + 12 + 24 + 48 + 4 + 64);
  REQUIRE(pdu.getMarshalledSize() == static_cast<int>(wire.size()));
  REQUIRE(U32At(wire, 132 + 12 + 24 + 48) == 4);  // M

  // Second variable datum: 96 bits of value, then 32 bits of padding.
  const std::size_t datum2 = 132 + 12 + 24 + 48 + 4 + 16;
  REQUIRE(U32At(wire, datum2) == 0x200u);
  REQUIRE(U32At(wire, datum2 + 4) == 96u);
  REQUIRE(wire[datum2 + 8] == 1);
  REQUIRE(wire[datum2 + 19] == 12);
  REQUIRE(U32At(wire, datum2 + 20) == 0u);

  // Exact read position, also when more octets follow the PDU.
  Bytes longer = wire;
  longer.push_back(0xEE);
  longer.push_back(0xEE);
  DIS::AggregateStatePdu got;
  REQUIRE(Unmarshal(longer, got) == wire.size());
  REQUIRE(got.getSilentEntitySystemList().size() == 3);
  REQUIRE(got.getSilentEntitySystemList()[2].getAppearanceRecordList()[1] == 0x00800000u);
  REQUIRE(got.getVariableDatumList().size() == 4);
  REQUIRE(got.getVariableDatumList()[2].getVariableDatumLength() == 64u);
  REQUIRE(got == pdu);
  REQUIRE(Marshal(got) == wire);
}

TEST_CASE("dis7 AggregateStatePdu truncated input throws", "[dis7][AggregateStatePdu]")
{
  const Bytes wire(kAggregateState, kAggregateState + sizeof(kAggregateState));
  const std::size_t cuts[] = {wire.size() - 1, 170, 160, 139, kAggregateFixedOctets};
  for (std::size_t i = 0; i < sizeof(cuts) / sizeof(cuts[0]); ++i)
  {
    INFO("cut at " << cuts[i]);
    const Bytes shorter(wire.begin(), wire.begin() + static_cast<std::ptrdiff_t>(cuts[i]));
    DIS::AggregateStatePdu got;
    REQUIRE_THROWS(Unmarshal(shorter, got));
  }
}

TEST_CASE("dis7 AggregateStatePdu refuses lists larger than their count fields", "[dis7][AggregateStatePdu]")
{
  DIS::AggregateStatePdu p;
  p.getEntityIDList().resize(65536);
  DIS::DataStream ds(DIS::BIG);
  REQUIRE_THROWS_AS(p.marshal(ds), std::length_error);
  REQUIRE(ds.size() == 0);

  DIS::SilentEntitySystem s;
  s.getAppearanceRecordList().resize(65536);
  REQUIRE_THROWS_AS(s.marshal(ds), std::length_error);
}

// Variable Datum record (6.2.93): ID (32), Length K in bits (32), value of
// ceil(K/8) octets padded to a 64-bit boundary.
TEST_CASE("dis7 VariableDatum reads a partial-octet length to the right boundary", "[dis7][VariableDatum]")
{
  // K = 65 bits: 9 value octets, padded to 16. A reader that takes floor(K/8)
  // octets reads 8 and leaves the next record 8 octets early.
  Bytes wire = {0x00, 0x00, 0x00, 0x2A,   // Variable Datum ID 42
                0x00, 0x00, 0x00, 0x41};  // Variable Datum Length 65 bits
  for (int i = 0; i < 16; ++i)
  {
    wire.push_back(i < 9 ? static_cast<unsigned char>(0xA0 + i) : 0x00);
  }
  wire.push_back(0xEE);  // first octet of whatever follows the record

  DIS::VariableDatum datum;
  REQUIRE(Unmarshal(wire, datum) == 24u);
  REQUIRE(datum.getVariableDatumLength() == 65u);
  REQUIRE(datum.getMarshalledSize() == 24);
  REQUIRE(Marshal(datum) == Bytes(wire.begin(), wire.begin() + 24));
}

TEST_CASE("dis7 VariableDatum marshals a long value with its padding in bounds", "[dis7][VariableDatum]")
{
  // 130 octets: more than the record's initial buffer and not a multiple of 8,
  // so marshal writes 136 octets and the buffer must hold them.
  std::vector<char> value(130, 0x5A);
  DIS::VariableDatum datum;
  datum.setVariableDatumID(7);
  datum.setVariableDatums(value.data(), static_cast<unsigned int>(value.size()));

  const Bytes wire = Marshal(datum);
  REQUIRE(wire.size() == 8u + 136u);
  REQUIRE(datum.getMarshalledSize() == 144);
  for (std::size_t i = 8; i < 8 + 130; ++i)
  {
    REQUIRE(wire[i] == 0x5A);
  }
  for (std::size_t i = 8 + 130; i < wire.size(); ++i)
  {
    REQUIRE(wire[i] == 0x00);
  }

  DIS::VariableDatum back;
  REQUIRE(Unmarshal(wire, back) == wire.size());
  REQUIRE(Marshal(back) == wire);
}

TEST_CASE("dis7 VariableDatum refuses a length that wraps 32-bit arithmetic", "[dis7][VariableDatum][VariableDatumBounds]")
{
  // K = 0xFFFFFFFF bits needs 0x20000000 value octets. Computed as (K + 7) / 8
  // in 32 bits, K + 7 wraps to 6 and the record reads 0 octets, leaving the
  // stream 8 octets into the record's value.
  const unsigned char maxLength[] = {
    0x00, 0x00, 0x00, 0x01,                          // Variable Datum ID
    0xFF, 0xFF, 0xFF, 0xFF,                          // Variable Datum Length (K)
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,  // 8 value octets, far short of K
  };
  DIS::VariableDatum datum;
  REQUIRE_THROWS_AS(Unmarshal(Bytes(maxLength, maxLength + sizeof(maxLength)), datum), std::out_of_range);
  REQUIRE(datum.getVariableDatumLength() == 0u);
  REQUIRE(datum.getMarshalledSize() == 8);
}

TEST_CASE("dis7 VariableDatum refuses a large length before sizing its buffer", "[dis7][VariableDatum][VariableDatumBounds]")
{
  // A large K that does not wrap (about 512 MiB of value) with a short input
  // is refused before a buffer is sized for it: the record keeps its empty
  // value, where sizing first would leave the claimed length behind.
  const unsigned char largeLength[] = {
    0x00, 0x00, 0x00, 0x01,                          // Variable Datum ID
    0xFF, 0xFF, 0xFF, 0xF0,                          // Variable Datum Length (K)
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
  };
  DIS::VariableDatum datum2;
  REQUIRE_THROWS_AS(Unmarshal(Bytes(largeLength, largeLength + sizeof(largeLength)), datum2), std::out_of_range);
  REQUIRE(datum2.getVariableDatumLength() == 0u);
  REQUIRE(datum2.getMarshalledSize() == 8);

  // The same length check passes a record whose value is all there.
  const unsigned char exact[] = {
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x40,  // K = 64 bits
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
  };
  DIS::VariableDatum datum3;
  REQUIRE(Unmarshal(Bytes(exact, exact + sizeof(exact)), datum3) == sizeof(exact));
}

TEST_CASE("dis7 SignalPdu writes the unused bits of a partial last octet as zero", "[dis7][SignalPdu][SignalPaddingBits]")
{
  // Table 178: K data bits, then P padding bits to the 32-bit boundary. DIS
  // is most-significant-bit first, so with K = 9 the second octet holds 1
  // data bit (its high bit) and 7 padding bits.
  //
  //  off  size  field                                   value (hex)
  //  28    2    Data Length (K, bits)                   00 09
  //  32    1    Data, bits 0-7                          FF
  //  33    1    Data bit 8, then 7 padding bits         80
  //  34    2    Padding to 32-bit boundary              00 00
  DIS::SignalPdu p;
  const unsigned char data[] = {0xFF, 0xFF};
  p.setData(std::vector<uint8_t>(data, data + 2));
  p.setDataLength(9);

  const Bytes wire = Marshal(p);
  REQUIRE(wire.size() == 36);
  REQUIRE(U16At(wire, 28) == 9);
  const unsigned char expectedTail[] = {0xFF, 0x80, 0x00, 0x00};
  REQUIRE(Bytes(wire.begin() + 32, wire.end()) == Bytes(expectedTail, expectedTail + 4));
  REQUIRE(p.getData()[1] == 0xFF);  // the stored data is not changed

  // Padding bits are not data: the round trip compares equal.
  DIS::SignalPdu got;
  REQUIRE(Unmarshal(wire, got) == wire.size());
  REQUIRE(got.getData()[1] == 0x80);
  REQUIRE(got == p);

  // Every partial length of a single octet keeps only its high K bits.
  for (unsigned short k = 1; k < 8; ++k)
  {
    INFO("K = " << k);
    DIS::SignalPdu one;
    one.setData(std::vector<uint8_t>(1, 0xFF));
    one.setDataLength(k);
    REQUIRE(Marshal(one)[32] == static_cast<unsigned char>(0xFF << (8 - k)));
  }
}
