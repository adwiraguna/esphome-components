import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate_ir
from esphome.const import CONF_ID

AUTO_LOAD = ["climate_ir"]

samsunac_ns = cg.esphome_ns.namespace("samsungac")
SamsungAC = samsunac_ns.class_("SamsungAC", climate_ir.ClimateIR)

CONFIG_SCHEMA = climate_ir.climate_ir_with_receiver_schema(SamsungAC)


async def to_code(config):
    var = await climate_ir.new_climate_ir(config)
