#include "samsungac.h"
#include "esphome/core/log.h"
#include "esphome/components/remote_base/remote_base.h"

namespace esphome {
namespace samsungac {

static const char *const TAG = "climate.samsungac";

void SamsungAC::transmit_state() {
  
  const uint8_t power_on_state[SAMSUNGAC_EXTENDED_LENGTH] = {
      0x02, 0x92, 0x0F, 0x00, 0x00, 0x00, 0xF0,
      0x01, 0xD2, 0x0F, 0x00, 0x00, 0x00, 0x00,
      0x01, 0xE2, 0xFE, 0x71, 0x80, 0x11, 0xF0};

  const uint8_t power_off_state[SAMSUNGAC_EXTENDED_LENGTH] = {
      0x02, 0xB2, 0x0F, 0x00, 0x00, 0x00, 0xC0,
      0x01, 0xD2, 0x0F, 0x00, 0x00, 0x00, 0x00,
      0x01, 0x02, 0xFF, 0x71, 0x80, 0x11, 0xC0};

  const uint8_t normal_state[SAMSUNGAC_EXTENDED_LENGTH] = {
      0x02, 0x92, 0x0F, 0x00, 0x00, 0x00, 0xF0,
      0x01, 0x02, 0xAE, 0x71, 0x00, 0x15, 0xF0};
  
  SamsungProtocol remote_state;

  if( (mode_before_ == climate::CLIMATE_MODE_OFF && this->mode != climate::CLIMATE_MODE_OFF) || this->mode == climate::CLIMATE_MODE_OFF)
  {
    if(this->mode == climate::CLIMATE_MODE_OFF){
      std::memcpy(remote_state.raw, power_off_state, SAMSUNGAC_EXTENDED_LENGTH);
      remote_state.Power1 = SAMSUNGAC_POWER_OFF;
      remote_state.Power3 = SAMSUNGAC_POWER_OFF;
      remote_state.Mode2 = this->operation_mode_(mode_before_);
    }
    else {
      std::memcpy(remote_state.raw, power_on_state, SAMSUNGAC_EXTENDED_LENGTH);
      remote_state.Power1 = SAMSUNGAC_POWER_ON;
      remote_state.Power3 = SAMSUNGAC_POWER_ON;
      remote_state.Mode2 = this->operation_mode_(this->mode);
    }

    mode_before_ = this->mode;

    remote_state.Fan2 = this->fan_mode_();
    remote_state.Swing2 = this->swing_mode_();
    remote_state.Temp2 = this->target_temperature_();

    this->transmit_(remote_state, SAMSUNGAC_EXTENDED_LENGTH);
  }
  else{
    ESP_LOGD(TAG, "transmit non power");
    std::memcpy(remote_state.raw, normal_state, SAMSUNGAC_EXTENDED_LENGTH);

    remote_state.Power1 = SAMSUNGAC_POWER_ON;
    remote_state.Power2 = SAMSUNGAC_POWER_ON;
    
    remote_state.Mode = this->operation_mode_(this->mode);
    mode_before_ = this->mode;

    remote_state.Fan = this->fan_mode_();
    remote_state.Swing = this->swing_mode_();
    remote_state.Temp= this->target_temperature_();

    this->transmit_(remote_state, SAMSUNGAC_MESSAGE_LENGTH);
  }
}

uint8_t SamsungAC::operation_mode_(climate::ClimateMode mode){
  switch (mode) {
    case climate::CLIMATE_MODE_COOL:
      return SAMSUNGAC_MODE_COOL;
    case climate::CLIMATE_MODE_HEAT:
      return SAMSUNGAC_MODE_HEAT;
    case climate::CLIMATE_MODE_DRY:
      return SAMSUNGAC_MODE_DRY;
    case climate::CLIMATE_MODE_FAN_ONLY:
      return SAMSUNGAC_MODE_FAN;
    case climate::CLIMATE_MODE_AUTO:
    case climate::CLIMATE_MODE_HEAT_COOL:
    default:
      return SAMSUNGAC_MODE_AUTO;
  }
}

uint8_t SamsungAC::fan_mode_() {
  
  if(this->mode == climate::CLIMATE_MODE_HEAT_COOL) {return SAMSUNGAC_FAN_AUTO2;}
  
  if(this->mode == climate::CLIMATE_MODE_DRY) {return SAMSUNGAC_FAN_AUTO;}

  switch (this->fan_mode.value()){
    case climate::CLIMATE_FAN_HIGH:
      return SAMSUNGAC_FAN_HIGH;
    case climate::CLIMATE_FAN_MEDIUM:
      return SAMSUNGAC_FAN_MED;
    case climate::CLIMATE_FAN_LOW:
      return SAMSUNGAC_FAN_LOW;
    case climate::CLIMATE_FAN_AUTO:
    default:
      return SAMSUNGAC_FAN_AUTO;
  }

}

uint8_t SamsungAC::swing_mode_(){
  switch (this->swing_mode){
    case climate::CLIMATE_SWING_BOTH:
      return SAMSUNGAC_SWING_BOTH;
    case climate::CLIMATE_SWING_HORIZONTAL:
      return SAMSUNGAC_SWING_HORIZONTAL;
    case climate::CLIMATE_SWING_VERTICAL:
      return SAMSUNGAC_SWING_VERTICAL;
    case climate::CLIMATE_SWING_OFF:
    default:
      return SAMSUNGAC_SWING_OFF;
  }
}

uint8_t SamsungAC::target_temperature_(){
  return this->target_temperature - SAMSUNGAC_MIN_TEMP;
}

bool SamsungAC::on_receive(remote_base::RemoteReceiveData data) {
  
  SamsungProtocol remote_state;
  uint8_t nByte = 0;

  if (!data.expect_item(SAMSUNGAC_HEADER_MARK, SAMSUNGAC_HEADER_SPACE)){return false;}

  for (uint8_t section = 0; section <= (data.size() / (2 * 8)) / SAMSUNGAC_SECTION_LENGTH; section++) {
    if (!data.expect_item(SAMSUNGAC_SECTION_MARK, SAMSUNGAC_SECTION_SPACE)){return false;}

    for(uint8_t pos = 0; pos < SAMSUNGAC_SECTION_LENGTH; pos++){
      uint32_t byte = 0;
      for(uint8_t bit = 0; bit < 8; bit++) {
        if (data.expect_item(SAMSUNGAC_BIT_MARK, SAMSUNGAC_ONE_SPACE)) {
          byte |= 1 << bit;
        } else if (data.expect_item(SAMSUNGAC_BIT_MARK, SAMSUNGAC_ZERO_SPACE)) {
        } else {
          return false;
        }
      }
      remote_state.raw[(section * SAMSUNGAC_SECTION_LENGTH) + pos] = byte;
      nByte++;
    }

    if(!data.expect_mark(SAMSUNGAC_BIT_MARK)) { return false;}
    if(!data.expect_space(SAMSUNGAC_SECTION_GAP)) {break;}
  }

  if(!validChecksum(remote_state.raw, nByte))
  {
      ESP_LOGD(TAG, "Invalid checksum");
      return false;
  }

  uint8_t mode;
  uint8_t fan_mode;
  uint8_t swing_mode;
  uint8_t target_temperature;

  if(remote_state.Power1 == remote_state.Power3)
  {
    mode = (remote_state.Power3 == SAMSUNGAC_POWER_OFF? 5 : remote_state.Mode2);
    fan_mode = remote_state.Fan2;
    swing_mode = remote_state.Swing2;
    target_temperature = remote_state.Temp2;
  }
  else
  {
    mode = remote_state.Mode;
    fan_mode = remote_state.Fan;
    swing_mode = remote_state.Swing;
    target_temperature = remote_state.Temp;
  }

  switch(mode) {
    case SAMSUNGAC_MODE_AUTO :
      this->mode = climate::CLIMATE_MODE_HEAT_COOL;
      break;
    case SAMSUNGAC_MODE_COOL :
      if(!this->supports_cool_)
        return false;
      this->mode = climate::CLIMATE_MODE_COOL;
      break;
    case SAMSUNGAC_MODE_DRY :
      if(!this->supports_dry_)
        return false;
      this->mode = climate::CLIMATE_MODE_DRY;
      break;
    case SAMSUNGAC_MODE_FAN : 
      if(!this->supports_fan_only_)
        return false;
      this->mode = climate::CLIMATE_MODE_FAN_ONLY;
      break;
    case SAMSUNGAC_MODE_HEAT:
      if(!this->supports_heat_)
        return false;
      this->mode = climate::CLIMATE_MODE_HEAT;
      break;
    case 5:
      this->mode = climate::CLIMATE_MODE_OFF;
  }

  mode_before_ = this->mode;

  switch(fan_mode) {
    case SAMSUNGAC_FAN_AUTO2 :
    case SAMSUNGAC_FAN_AUTO :
      this->fan_mode = climate::CLIMATE_FAN_AUTO;
      break;
    case SAMSUNGAC_FAN_HIGH :
      this->fan_mode = climate::CLIMATE_FAN_HIGH;
      break;
    case SAMSUNGAC_FAN_MED :
      this->fan_mode = climate::CLIMATE_FAN_MEDIUM;
      break;
    case SAMSUNGAC_FAN_LOW : 
      this->fan_mode = climate::CLIMATE_FAN_LOW;
      break;
  }

  switch(swing_mode) {
    case SAMSUNGAC_SWING_BOTH :
      this->swing_mode = climate::CLIMATE_SWING_BOTH;
      break;
    case SAMSUNGAC_SWING_HORIZONTAL :
      this->swing_mode = climate::CLIMATE_SWING_HORIZONTAL;
      break;
    case SAMSUNGAC_SWING_OFF :
      this->swing_mode = climate::CLIMATE_SWING_OFF;
      break;
    case SAMSUNGAC_SWING_VERTICAL :
      this->swing_mode = climate::CLIMATE_SWING_VERTICAL;
      break;
  }

  this->target_temperature = target_temperature + SAMSUNGAC_MIN_TEMP;

  this->publish_state();
  return true;
}

void SamsungAC::transmit_(SamsungProtocol state, uint16_t length) {  

  checksum(state);

  auto transmit = this->transmitter_->transmit();
  auto *data = transmit.get_data();

  data->set_carrier_frequency(38000);

  data->item(SAMSUNGAC_HEADER_MARK, SAMSUNGAC_HEADER_SPACE);

  for(uint8_t section = 0; section < length / SAMSUNGAC_SECTION_LENGTH; section++){
    data->item(SAMSUNGAC_SECTION_MARK, SAMSUNGAC_SECTION_SPACE);
    for(uint8_t pos = 0; pos < SAMSUNGAC_SECTION_LENGTH; pos++)
    {
      for (uint8_t mask = 1; mask > 0; mask <<= 1) {  // iterate through bit mask
        data->mark(SAMSUNGAC_BIT_MARK);
        bool bit = state.raw[(section * SAMSUNGAC_SECTION_LENGTH) + pos] & mask;
        data->space(bit ? SAMSUNGAC_ONE_SPACE : SAMSUNGAC_ZERO_SPACE);
      }
    }
    data->mark(SAMSUNGAC_BIT_MARK);
    if(section != (length / SAMSUNGAC_SECTION_LENGTH) - 1) {
       data->space(SAMSUNGAC_SECTION_GAP);
    }
  }

  transmit.perform();
}

bool SamsungAC::validChecksum(const uint8_t state[], const uint16_t length) {
  bool result = true;
  const uint16_t maxlength =
      (length > SAMSUNGAC_EXTENDED_LENGTH) ? SAMSUNGAC_EXTENDED_LENGTH : length;
  for (uint16_t offset = 0;
       offset + SAMSUNGAC_SECTION_LENGTH <= maxlength;
       offset += SAMSUNGAC_SECTION_LENGTH)
    result &= (getSectionChecksum(state + offset) ==
               calcSectionChecksum(state + offset));
  return result;
}

uint8_t SamsungAC::getSectionChecksum(const uint8_t *section) {
  return ((GETBITS8(*(section + 2), 0, 4) << 4) +
          GETBITS8(*(section + 1), 4, 4));
}

uint8_t SamsungAC::calcSectionChecksum(const uint8_t *section) {
  uint8_t sum = 0;

  sum += countBits(*section, 8);  // Include the entire first byte
  // The lower half of the second byte.
  sum += countBits(GETBITS8(*(section + 1), 0, 4), 8);
  // The upper half of the third byte.
  sum += countBits(GETBITS8(*(section + 2), 4, 4), 8);
  // The next 4 bytes.
  sum += countBits(section + 3, 4);
  // Bitwise invert the result.
  return sum ^ UINT8_MAX;
}

uint16_t SamsungAC::countBits(const uint8_t * const start, const uint16_t length,
                   const bool ones, const uint16_t init) {
  uint16_t count = init;
  for (uint16_t offset = 0; offset < length; offset++)
    for (uint8_t currentbyte = *(start + offset);
         currentbyte;
         currentbyte >>= 1)
      if (currentbyte & 1) count++;
  if (ones || length == 0)
    return count;
  else
    return (length * 8) - count;
}

uint16_t SamsungAC::countBits(const uint64_t data, const uint8_t length, const bool ones,
                   const uint16_t init) {
  uint16_t count = init;
  uint8_t bitsSoFar = length;
  for (uint64_t remainder = data; remainder && bitsSoFar;
       remainder >>= 1, bitsSoFar--)
      if (remainder & 1) count++;
  if (ones || length == 0)
    return count;
  else
    return length - count;
}

void SamsungAC::checksum(SamsungProtocol & state) {
  uint8_t sectionsum = calcSectionChecksum(state.raw);
  state.Sum1Upper = GETBITS8(sectionsum, 4, 4);
  state.Sum1Lower = GETBITS8(sectionsum, 0, 4);
  sectionsum = calcSectionChecksum(state.raw + SAMSUNGAC_SECTION_LENGTH);
  state.Sum2Upper = GETBITS8(sectionsum, 4, 4);
  state.Sum2Lower = GETBITS8(sectionsum, 0, 4);
  sectionsum = calcSectionChecksum(state.raw + SAMSUNGAC_SECTION_LENGTH * 2);
  state.Sum3Upper = GETBITS8(sectionsum, 4, 4);
  state.Sum3Lower = GETBITS8(sectionsum, 0, 4);
}

}  // namespace samsungac
}  // namespace esphome