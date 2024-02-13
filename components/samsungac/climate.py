import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate_ir
from esphome.const import CONF_ID

AUTO_LOAD = ["climate_ir"]

samsunac_ns = cg.esphome_ns.namespace("samsungac")
SamsungAC = samsunac_ns.class_("SamsungAC", climate_ir.ClimateIR)

#CONF_HEADER_HIGH = "header_high"
#CONF_HEADER_LOW = "header_low"
#CONF_BIT_HIGH = "bit_high"
#CONF_BIT_ONE_LOW = "bit_one_low"
#CONF_BIT_ZERO_LOW = "bit_zero_low"

CONFIG_SCHEMA = climate_ir.CLIMATE_IR_WITH_RECEIVER_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(SamsungAC),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await climate_ir.register_climate_ir(var, config)