#pragma once

#include "esphome/components/climate_ir/climate_ir.h"

#include <cinttypes>

namespace esphome {
namespace climate_ir_lg_x {

// Temperature
const uint8_t TEMP_MIN = 18;  // Celsius
const uint8_t TEMP_MAX = 30;  // Celsius

class LgIrClimateX : public climate_ir::ClimateIR {
 public:
  LgIrClimateX()
      : climate_ir::ClimateIR(TEMP_MIN, TEMP_MAX, 1.0f, true, false,
                              {climate::CLIMATE_FAN_AUTO, climate::CLIMATE_FAN_LOW, climate::CLIMATE_FAN_MEDIUM,
                               climate::CLIMATE_FAN_HIGH},
                              {climate::CLIMATE_SWING_OFF, climate::CLIMATE_SWING_VERTICAL}) {}

  /// Override control to change settings of the climate device.
  void control(const climate::ClimateCall &call) override {
    // This protocol only encodes one command per IR frame (COMMAND_SWING is a stateless
    // toggle, distinct from COMMAND_COOL/HEAT/etc.), so bundling a swing change into the
    // same call as a mode/fan/temp change would silently drop the latter if we always
    // took the swing branch. Only treat this as a swing-toggle transmission when swing
    // is the only thing changing, and it's an actual change from the current value (not
    // a resend of it, which would otherwise flip the toggle the wrong way).
    bool swing_requested = call.get_swing_mode().has_value() && *call.get_swing_mode() != this->swing_mode;
    bool other_change_requested =
        call.get_mode().has_value() || call.get_target_temperature().has_value() || call.get_fan_mode().has_value();
    send_swing_cmd_ = swing_requested && !other_change_requested;

    // swing resets after unit powered off
    if (call.get_mode().has_value() && *call.get_mode() == climate::CLIMATE_MODE_OFF)
      this->swing_mode = climate::CLIMATE_SWING_OFF;
    climate_ir::ClimateIR::control(call);
  }
  void set_header_high(uint32_t header_high) { this->header_high_ = header_high; }
  void set_header_low(uint32_t header_low) { this->header_low_ = header_low; }
  void set_bit_high(uint32_t bit_high) { this->bit_high_ = bit_high; }
  void set_bit_one_low(uint32_t bit_one_low) { this->bit_one_low_ = bit_one_low; }
  void set_bit_zero_low(uint32_t bit_zero_low) { this->bit_zero_low_ = bit_zero_low; }

 protected:
  /// Transmit via IR the state of this climate controller.
  void transmit_state() override;
  /// Handle received IR Buffer
  bool on_receive(remote_base::RemoteReceiveData data) override;

  bool send_swing_cmd_{false};

  void calc_checksum_(uint32_t &value);
  bool verify_checksum_(uint32_t value);
  void transmit_(uint32_t value);

  uint32_t header_high_;
  uint32_t header_low_;
  uint32_t bit_high_;
  uint32_t bit_one_low_;
  uint32_t bit_zero_low_;

  climate::ClimateMode mode_before_{climate::CLIMATE_MODE_OFF};
};

}  // namespace climate_ir_lg_x
}  // namespace esphome
