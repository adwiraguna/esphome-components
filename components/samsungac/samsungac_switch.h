#pragma once

#include "esphome/components/switch/switch.h"
#include "samsungac.h"

namespace esphome {
namespace samsungac {

enum class SamsungACSwitchType {
  DISPLAY,
  ION,
  BEEP,
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
      case SamsungACSwitchType::DISPLAY:
        this->parent_->set_display(state);
        break;
      case SamsungACSwitchType::ION:
        this->parent_->set_ion(state);
        break;
      case SamsungACSwitchType::BEEP:
        this->parent_->set_beep(state);
        break;
    }
    this->publish_state(state);
  }

  SamsungAC *parent_{nullptr};
  SamsungACSwitchType type_{SamsungACSwitchType::DISPLAY};
};

}  // namespace samsungac
}  // namespace esphome
