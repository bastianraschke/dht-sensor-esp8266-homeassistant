# DHT MQTT sensor on ESP8266 for Home Assistant

## Features

- DHT11/DHT21/DHT22 sensor firmware for ESP8266 (e.g. NodeMCU)
- Easy integration to Home Assistant
- Encrypted MQTT communication with TLS 1.1
- Deep sleep is supported for low power consumption
- Customizable data send interval

## Project application

Here you see the firmware used on a NodeMCU to measure temperature and humidity values in my appartment:

<img alt="DHT11 sensor connected to ESP8266 running MQTT client" src="https://github.com/bastianraschke/dht-sensor-esp8266-homeassistant/blob/master/projectcover.jpg" width="650">

## Configuration

The firmware must be configured before flashing to ESP8266. Rename `src/Firmware/sketches/Production/config-sample.h` to `src/Firmware/sketches/Production/config.h` and change the values like desired.

## Example configuration for Home Assistant

This example must be added to the `sensor` block of your configuration.

    - platform: mqtt
      state_topic: '/sensor/api/1/id/AAAABBBB/state/'
      name: 'Temperature'
      unit_of_measurement: '°C'
      value_template: '{{ value_json.temperature }}'

    - platform: mqtt
      state_topic: '/sensor/api/1/id/AAAABBBB/state/'
      name: 'Humidity'
      unit_of_measurement: '%'
      value_template: '{{ value_json.humidity }}'
