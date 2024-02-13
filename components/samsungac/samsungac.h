#pragma once

#include "esphome/components/climate_ir/climate_ir.h"

#include <cinttypes>

namespace esphome {
namespace samsungac {

#define GETBITS8(data, offset, size) \
    (((data) & (((uint8_t)UINT8_MAX >> (8 - (size))) << (offset))) >> (offset))

union SamsungProtocol{
  uint8_t raw[21];  ///< State in code form.
  struct {  // Standard message map
    // Byte 0
    uint8_t         :8; //01000000
    // Byte 1
    uint8_t         :8; //01001001
    // Byte 2
    uint8_t         :8; //11110000
    // Byte 3
    uint8_t         :8; //00000000
    // Byte 4
    uint8_t         :8; //00000000
    // Byte 5
    uint8_t         :8; //00000000
    // Byte 6
    uint8_t         :4; //0000
    uint8_t Power1  :2; //power 11 = on 00 = off
    uint8_t         :2; //11
    // Byte 7
    uint8_t         :8; //10000000
    // Byte 8
    uint8_t         :4; //0100
    uint8_t         :4; // Sum1Lower
    // Byte 9
    uint8_t         :4; // Sum1Upper
    uint8_t Swing   :3; //000 = off, 010 = on
    uint8_t         :1; 
    // Byte 10
    uint8_t               :1;
    uint8_t FanSpecial    :3;  // 0000 Powerful, Breeze/WindFree, Econo
    uint8_t Display       :1;  
    uint8_t               :2;  
    uint8_t CleanToggle10 :1;
    // Byte 11
    uint8_t Ion           :1;
    uint8_t CleanToggle11 :1;
    uint8_t               :2;
    uint8_t Temp          :4;
    // Byte 12
    uint8_t       :1;
    uint8_t Fan   :3;
    uint8_t Mode  :3; //Auto = 000, Cool = 100 (4), Dry = 010 (2), Fan = 110 (6), Heat = 001 (1)
    uint8_t       :1;
    // Byte 13
    uint8_t            :2;
    uint8_t BeepToggle :1;
    uint8_t            :1;
    uint8_t Power2     :2;
    uint8_t            :2;
  };
  struct {  // Extended message map
    // 1st Section
    // Byte 0
    uint8_t                :8;
    // Byte 1
    uint8_t                :4;
    uint8_t Sum1Lower      :4;
    // Byte 2
    uint8_t Sum1Upper      :4;
    uint8_t                :4;
    // Byte 3
    uint8_t                :8;
    // Byte 4
    uint8_t                :8;
    // Byte 5
    uint8_t                :8;
    // Byte 6
    uint8_t                :8;
    // 2nd Section
    // Byte 7
    uint8_t                :8;
    // Byte 8
    uint8_t                :4;
    uint8_t Sum2Lower      :4;
    // Byte 9
    uint8_t Sum2Upper      :4;
    uint8_t OffTimeMins    :3;  // In units of 10's of mins
    uint8_t OffTimeHrs1    :1;  // LSB of the number of hours.
    // Byte 10
    uint8_t OffTimeHrs2    :4;  // MSBs of the number of hours.
    uint8_t OnTimeMins     :3;  // In units of 10's of mins
    uint8_t OnTimeHrs1     :1;  // LSB of the number of hours.
    // Byte 11
    uint8_t OnTimeHrs2     :4;  // MSBs of the number of hours.
    uint8_t                :4;
    // Byte 12
    uint8_t OffTimeDay     :1;
    uint8_t OnTimerEnable  :1;
    uint8_t OffTimerEnable :1;
    uint8_t Sleep12        :1;
    uint8_t OnTimeDay      :1;
    uint8_t                :3;
    // Byte 13
    uint8_t                :8;
    // 3rd Section
    // Byte 14
    uint8_t                :8;
    // Byte 15
    uint8_t                :4;
    uint8_t Sum3Lower      :4;
    // Byte 16
    uint8_t Sum3Upper      :4;
    uint8_t Swing2         :3; //000 = off, 010 = on
    uint8_t                :1; 
    // Byte 17
    uint8_t                :8;
    // Byte 18
    uint8_t                :4;
    uint8_t Temp2          :4;
    // Byte 19
    uint8_t                :1;
    uint8_t Fan2           :3;
    uint8_t Mode2          :3; //Auto = 000, Cool = 100 (4), Dry = 010 (2), Fan = 110 (6), Heat = 001 (1)
    uint8_t                :1;
    // Byte 20
    uint8_t                :4;
    uint8_t Power3         :2;
    uint8_t                :2;
  };
  
};

// Temperature
const uint8_t TEMP_MIN = 16;  // Celsius
const uint8_t TEMP_MAX = 30;  // Celsius

const uint32_t SAMSUNGAC_HEADER_MARK = 600;
const uint32_t SAMSUNGAC_HEADER_SPACE = 18000;
const uint8_t SAMSUNGAC_SECTIONS = 2;
const uint16_t SAMSUNGAC_SECTION_MARK = 3000;
const uint16_t SAMSUNGAC_SECTION_SPACE = 9000;
const uint16_t SAMSUNGAC_SECTION_GAP = 3000;
const uint16_t SAMSUNGAC_BIT_MARK = 530;
const uint16_t SAMSUNGAC_ONE_SPACE = 1470;
const uint16_t SAMSUNGAC_ZERO_SPACE = 470;
const uint16_t SAMSUNGAC_SECTION_LENGTH = 7;
const uint16_t SAMSUNGAC_MESSAGE_LENGTH = 14;
const uint16_t SAMSUNGAC_EXTENDED_LENGTH = 21;


const uint8_t SAMSUNGAC_MIN_TEMP  = 16;  // C   Mask 0b11110000
const uint8_t kSamsungAcMaxTemp  = 30;  // C   Mask 0b11110000
const uint8_t kSamsungAcAutoTemp = 25;  // C   Mask 0b11110000
const uint8_t SAMSUNGAC_MODE_AUTO = 0;
const uint8_t SAMSUNGAC_MODE_COOL = 1;
const uint8_t SAMSUNGAC_MODE_DRY = 2;
const uint8_t SAMSUNGAC_MODE_FAN = 3;
const uint8_t SAMSUNGAC_MODE_HEAT = 4;
const uint8_t SAMSUNGAC_FAN_AUTO = 0;
const uint8_t SAMSUNGAC_FAN_LOW = 2;
const uint8_t SAMSUNGAC_FAN_MED = 4;
const uint8_t SAMSUNGAC_FAN_HIGH = 5;
const uint8_t SAMSUNGAC_FAN_AUTO2 = 6;
const uint8_t SAMSUNGAC_FAN_TURBO = 7;

// _.Swing
const uint8_t SAMSUNGAC_POWER_OFF = 0b000; //
const uint8_t SAMSUNGAC_POWER_ON = 0b011; //

// _.Swing
const uint8_t SAMSUNGAC_SWING_VERTICAL = 0b010; //2
const uint8_t SAMSUNGAC_SWING_HORIZONTAL = 0b011; //3
const uint8_t SAMSUNGAC_SWING_BOTH = 0b100; //4
const uint8_t SAMSUNGAC_SWING_OFF = 0b111; //7
// _.FanSpecial
const uint8_t kSamsungAcFanSpecialOff = 0b000;
const uint8_t kSamsungAcPowerfulOn =    0b011;
const uint8_t kSamsungAcBreezeOn =      0b101;
const uint8_t kSamsungAcEconoOn =       0b111;


class SamsungAC : public climate_ir::ClimateIR {
 public:
  SamsungAC()
      : climate_ir::ClimateIR(TEMP_MIN, TEMP_MAX, 1.0f, true, true,
                              {climate::CLIMATE_FAN_AUTO, climate::CLIMATE_FAN_LOW, climate::CLIMATE_FAN_MEDIUM,
                               climate::CLIMATE_FAN_HIGH},
                              {climate::CLIMATE_SWING_OFF, climate::CLIMATE_SWING_VERTICAL}) {}
  void control(const climate::ClimateCall &call){
    if(call.get_fan_mode().has_value() 
        && *call.get_fan_mode() != climate::CLIMATE_FAN_AUTO 
        && (this->mode == climate::CLIMATE_MODE_AUTO || this->mode == climate::CLIMATE_MODE_DRY )){
      return;
    }

    climate_ir::ClimateIR::control(call);
  }

 protected:
  /// Transmit via IR the state of this climate controller.
  void transmit_state() override;
  /// Handle received IR Buffer
  bool on_receive(remote_base::RemoteReceiveData data) override;

 private:
  void transmit_(SamsungProtocol state, uint16_t length);
  bool validChecksum(const uint8_t state[], const uint16_t length);
  uint8_t getSectionChecksum(const uint8_t *section);
  uint8_t calcSectionChecksum(const uint8_t *section);
  void checksum(SamsungProtocol & state);
  uint16_t countBits(const uint8_t * const start, const uint16_t length,
                   const bool ones = true, const uint16_t init = 0);
  uint16_t countBits(const uint64_t data, const uint8_t length,
                   const bool ones = true, const uint16_t init = 0);
  uint8_t operation_mode_(climate::ClimateMode mode);
  uint8_t fan_mode_();
  uint8_t swing_mode_();
  uint8_t target_temperature_();

  climate::ClimateMode mode_before_{climate::CLIMATE_MODE_OFF}; 
};

}  // namespace samsungac
}  // namespace esphome
