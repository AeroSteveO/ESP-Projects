import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, output, time
from esphome.const import CONF_OUTPUT_ID, CONF_OUTPUT, CONF_TIME_ID, CONF_PIN, CONF_NUM_LEDS
from esphome import pins

wordclock_ns = cg.esphome_ns.namespace('wordclock')
WordClock = wordclock_ns.class_('WordClock', light.AddressableLight)

CONFIG_SCHEMA = cv.All(light.ADDRESSABLE_LIGHT_SCHEMA.extend({
            cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(WordClock),
            cv.Required(CONF_TIME_ID): cv.use_id(time.RealTimeClock),
            cv.Required(CONF_PIN): pins.internal_gpio_output_pin_number, 
            cv.Required(CONF_NUM_LEDS): cv.positive_not_null_int,
        #    cv.Required(CONF_OUTPUT): cv.use_id(output.FloatOutput)
        }
    ).extend(cv.COMPONENT_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await light.register_light(var, config)
    await cg.register_component(var, config)
    
    if CONF_TIME_ID in config:
        time_ = await cg.get_variable(config[CONF_TIME_ID])
        cg.add(var.set_time_id(time_))
    
    if CONF_PIN in config:
        cg.add(var.add_leds(config[CONF_NUM_LEDS], config[CONF_PIN]))
    
    # https://github.com/Makuna/NeoPixelBus/blob/master/library.json
    # Version Listed Here: https://registry.platformio.org/libraries/makuna/NeoPixelBus/versions
    cg.add_library("adafruit/Adafruit NeoPixel", "Latest")
#    out = yield cg.get_variable(config[CONF_OUTPUT])
#    cg.add(var.set_output(out))