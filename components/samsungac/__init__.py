import esphome.codegen as cg
from esphome.components import climate_ir

CODEOWNERS = ["@adwiraguna"]

samsungac_ns = cg.esphome_ns.namespace("samsungac")
SamsungAC = samsungac_ns.class_("SamsungAC", climate_ir.ClimateIR)
