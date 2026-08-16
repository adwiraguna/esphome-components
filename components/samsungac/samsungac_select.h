#pragma once

#include "esphome/components/select/select.h"
#include "samsungac.h"

namespace esphome {
namespace samsungac {

/// A select entity offering the mutually-exclusive "Off"/"Powerful"/"WindFree"/"Econo"
/// special fan modes, forwarded straight to SamsungAC::set_special_mode().
class SamsungACSpecialModeSelect : public select::Select {
 public:
  void set_parent(SamsungAC *parent) { this->parent_ = parent; }

 protected:
  void control(const std::string &value) override {
    this->parent_->set_special_mode(value);
    this->publish_state(value);
  }

  SamsungAC *parent_{nullptr};
};

}  // namespace samsungac
}  // namespace esphome
