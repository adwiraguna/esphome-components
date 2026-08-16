# adwiraguna's esphome custom components

This repository contains a collection of my custom components for [ESPHome](https://esphome.io/).

If you like my work Buy me a coffee.

[!["Buy Me A Coffee"](https://github.com/adwiraguna/esphome-components/blob/main/images/bmc_qr_small.png)](https://www.buymeacoffee.com/adwiraguna)

## 1. Usage

Use latest [ESPHome](https://esphome.io/) with external components and add this to your `.yaml` definition:

```yaml
external_components:
  - source: github://adwiraguna/esphome-components@main
```

## 2. Components

### 2.1. `samsungac`

This climate component allows you to control Samsung AC units by sending an infrared (IR) control signal, just as the unit’s handheld remote controller would.
You need to set receiver idle time to above 18ms.

#### 2.1.1. Example

```yaml
external_components:
  - source: github://adwiraguna/esphome-components@main
    components: [ samsungac ]

remote_receiver:
  id: ir_receiver
  pin:
    number: GPIO5
    inverted: true
  tolerance: 55%
  idle: 20ms

climate:
  - platform: samsungac
    name: "Living Room AC"
    receiver_id: ir_receiver
    id: living_room_ac
```

#### 2.1.2. Extra switches and select

Besides the climate entity itself, the component optionally exposes the unit's
display, ioniser, and beeper toggles as `switch`, and the mutually-exclusive
Powerful/WindFree/Econo fan modes as a `select`. Both reference the climate
entity via `samsungac_id`. These are separate IR commands sent immediately
when changed — they only take effect while the unit is on.

```yaml
switch:
  - platform: samsungac
    samsungac_id: living_room_ac
    display:
      name: "Living Room AC Display"
    ion:
      name: "Living Room AC Ioniser"
    beep:
      name: "Living Room AC Beep"

select:
  - platform: samsungac
    samsungac_id: living_room_ac
    name: "Living Room AC Special Mode"
```

The special-mode select offers `Off`, `Powerful`, `WindFree`, and `Econo`.
Selecting `Powerful` or `Econo` also overrides the fan speed/swing (and
`WindFree` the swing) to match what the physical remote sends for that mode.

#### 2.1.3. Credits
Based on [IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266)