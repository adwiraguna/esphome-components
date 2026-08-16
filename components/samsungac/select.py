import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select
from esphome.const import ENTITY_CATEGORY_CONFIG

from . import SamsungAC, samsungac_ns

CONF_SAMSUNGAC_ID = "samsungac_id"

SamsungACSpecialModeSelect = samsungac_ns.class_(
    "SamsungACSpecialModeSelect", select.Select
)

# Order matches the C++ side's string comparisons in SamsungAC::set_special_mode().
SPECIAL_MODE_OPTIONS = ["Off", "Powerful", "WindFree", "Econo"]

CONFIG_SCHEMA = select.select_schema(
    SamsungACSpecialModeSelect, entity_category=ENTITY_CATEGORY_CONFIG
).extend(
    {
        cv.GenerateID(CONF_SAMSUNGAC_ID): cv.use_id(SamsungAC),
    }
)


async def to_code(config):
    var = await select.new_select(config, options=SPECIAL_MODE_OPTIONS)
    parent = await cg.get_variable(config[CONF_SAMSUNGAC_ID])
    cg.add(var.set_parent(parent))
