import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

wordclock_ns = cg.esphome_ns.namespace('wordclock')
WordClock = wordclock_ns.class_('WordClock', cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(WordClock)
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)

#    if CONF_TEMPERATURE in config:
#        sens = await sensor.new_sensor(config[CONF_TEMPERATURE])
#        cg.add(var.set_temperature_sensor(sens))
