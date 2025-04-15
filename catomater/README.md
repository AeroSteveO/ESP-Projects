 * for some reason, the laser switch in HA turns off after being turned on, even if the laser is still on, calling switch.turn_off will turn it off even if the current status shows off
 * i've included my adjusted laser adapter model in here as well in case you have a 12mm dia laser
 * original models: https://www.printables.com/model/579611-the-catomater-v-19-a-cat-automater-maybe-ceilingwa/
 * i'll eventually flesh this out a bit more


```
 alias: Cat Laser - Park position
 sequence:
  - service: switch.turn_off
    data: {}
    target:
      entity_id: switch.cat_laser_laser_pointer
  - variables:
      park_x: -25
      park_y: 75
  - service: esphome.cat_laser_servo_x
    data:
       level: "{{ park_x }}"
  - service: esphome.cat_laser_servo_y
    data:
       level: "{{ park_y }}"
 mode: single
 icon: mdi:cat
```