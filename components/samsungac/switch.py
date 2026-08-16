import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import CONF_ID, ENTITY_CATEGORY_CONFIG

from . import SamsungAC, samsungac_ns

CONF_SAMSUNGAC_ID = "samsungac_id"
CONF_DISPLAY = "display"
CONF_ION = "ion"
CONF_BEEP = "beep"

SamsungACSwitch = samsungac_ns.class_("SamsungACSwitch", switch.Switch)
SamsungACSwitchType = samsungac_ns.enum("SamsungACSwitchType", is_class=True)

SWITCH_TYPES = {
    CONF_DISPLAY: SamsungACSwitchType.SWITCH_DISPLAY,
    CONF_ION: SamsungACSwitchType.SWITCH_ION,
    CONF_BEEP: SamsungACSwitchType.SWITCH_BEEP,
}

# A single `switch:` platform entry here can yield up to three sub-switches (display/
# ion/beep), so it doesn't map onto a single switch_schema() itself; it needs its own
# placeholder id, same as e.g. the ld2410 component's multi-switch platform entry does.
CONFIG_SCHEMA = {
    cv.GenerateID(CONF_ID): cv.declare_id(cg.EntityBase),
    cv.GenerateID(CONF_SAMSUNGAC_ID): cv.use_id(SamsungAC),
    cv.Optional(CONF_DISPLAY): switch.switch_schema(
        SamsungACSwitch, entity_category=ENTITY_CATEGORY_CONFIG
    ),
    cv.Optional(CONF_ION): switch.switch_schema(
        SamsungACSwitch, entity_category=ENTITY_CATEGORY_CONFIG
    ),
    cv.Optional(CONF_BEEP): switch.switch_schema(
        SamsungACSwitch, entity_category=ENTITY_CATEGORY_CONFIG
    ),
}


async def to_code(config):
    parent = await cg.get_variable(config[CONF_SAMSUNGAC_ID])
    for key, switch_type in SWITCH_TYPES.items():
        if (conf := config.get(key)) is None:
            continue
        var = await switch.new_switch(conf)
        cg.add(var.set_parent(parent))
        cg.add(var.set_switch_type(switch_type))
