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
```
#### 2.1.2. Credits
Based on [IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266)