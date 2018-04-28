# DHT11 sensor via MQTT on ESP8266 for Home Assistant

## Example configuration

This example must be added to the `sensor` block of your configuration.

  sensor:
    - platform: mqtt
      state_topic: '/sensor/api/5/id/AAAABBBB/state/'
      name: 'Temperature'
      unit_of_measurement: '°C'
      value_template: '{{ value_json.temperature }}'

    - platform: mqtt
      state_topic: '/sensor/api/5/id/AAAABBBB/state/'
      name: 'Humidity'
      unit_of_measurement: '%'
      value_template: '{{ value_json.humidity }}'
