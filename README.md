# DHT MQTT sensor on ESP8266 for Home Assistant

## Features

- DHT11/DHT21/DHT22 sensor firmware for ESP8266 (NodeMCU)
- Easy integration to Home Assistant
- MQTT communication is encrypted with TLS 1.1
- Deep sleep is supported for low power consumption
- Customizable data send interval

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
