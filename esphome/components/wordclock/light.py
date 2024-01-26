import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, output, time
from esphome.const import CONF_OUTPUT_ID, CONF_OUTPUT, CONF_TIME_ID


empty_light_ns = cg.esphome_ns.namespace('wordclock')
WordClock = empty_light_ns.class_('WordClock', light.LightOutput)

CONFIG_SCHEMA = light.ADDRESSABLE_LIGHT_SCHEMA.extend({
    cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(WordClock),
    cv.Required(CONF_TIME_ID): cv.use_id(time.RealTimeClock),
#    cv.Required(CONF_OUTPUT): cv.use_id(output.FloatOutput)
})

def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    yield light.register_light(var, config)
    
    if CONF_TIME_ID in config:
        time_ = await cg.get_variable(config[CONF_TIME_ID])
        cg.add(var.set_time_id(time_))
        
#    out = yield cg.get_variable(config[CONF_OUTPUT])
#    cg.add(var.set_output(out))