from esphome.components import climate_ir

from . import SamsungAC

AUTO_LOAD = ["climate_ir"]

CONFIG_SCHEMA = climate_ir.climate_ir_with_receiver_schema(SamsungAC)


async def to_code(config):
    await climate_ir.new_climate_ir(config)
