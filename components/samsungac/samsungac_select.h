#pragma once

// external_components syncs and compiles this whole directory as one unit, regardless
// of which of samsungac's platforms (climate/switch/select) your YAML actually
// configures. Guard on USE_SELECT (defined only once something in your config loads
// ESPHome's core `select` component) so this file compiles to nothing rather than
// failing with a missing esphome/components/select/select.h if you haven't added a
// `select:` block anywhere.
#include "esphome/core/defines.h"

#ifdef USE_SELECT

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

#endif  // USE_SELECT
