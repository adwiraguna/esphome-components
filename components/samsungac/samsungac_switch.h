#pragma once

#include "esphome/components/switch/switch.h"
#include "samsungac.h"

namespace esphome {
namespace samsungac {

// Plain all-caps names here (DISPLAY, ION, BEEP, ...) are asking for trouble: the
// ESP8266 Arduino core's Arduino.h #defines DISPLAY as 0x1, and since macros are a
// pure text substitution, `DISPLAY,` inside this enum got replaced with `0x1,` and
// broke the build. Prefixed names sidestep that (and any similar future collision).
enum class SamsungACSwitchType {
  SWITCH_DISPLAY,
  SWITCH_ION,
  SWITCH_BEEP,
};

/// A single switch entity that toggles one of SamsungAC's extra state bits
/// (display / ion / beep) and pushes an immediate IR update via the parent.
class SamsungACSwitch : public switch_::Switch {
 public:
  void set_parent(SamsungAC *parent) { this->parent_ = parent; }
  void set_switch_type(SamsungACSwitchType type) { this->type_ = type; }

 protected:
  void write_state(bool state) override {
    switch (this->type_) {
      case SamsungACSwitchType::SWITCH_DISPLAY:
        this->parent_->set_display(state);
        break;
      case SamsungACSwitchType::SWITCH_ION:
        this->parent_->set_ion(state);
        break;
      case SamsungACSwitchType::SWITCH_BEEP:
        this->parent_->set_beep(state);
        break;
    }
    this->publish_state(state);
  }

  SamsungAC *parent_{nullptr};
  SamsungACSwitchType type_{SamsungACSwitchType::SWITCH_DISPLAY};
};

}  // namespace samsungac
}  // namespace esphome
