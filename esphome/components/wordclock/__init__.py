import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, output
from esphome.const import CONF_ID, CONF_OUTPUT_ID, CONF_OUTPUT, CONF_LIGHT

wordclock_ns = cg.esphome_ns.namespace('wordclock')
WordClock = wordclock_ns.class_('WordClock', cg.Component)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(WordClock),
            cv.Required(CONF_LIGHT): light.AddressableLight(),
        },
    )
).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])    
    yield cg.register_component(var, config)   
    
    if CONF_LIGHT in config:
        light = yield light.register_light(var, config)
        cg.add(var.set_clock_face_lights(light))
    
#    if CONF_TEMPERATURE in config:
#        sens = await sensor.new_sensor(config[CONF_TEMPERATURE])
#        cg.add(var.set_temperature_sensor(sens))
