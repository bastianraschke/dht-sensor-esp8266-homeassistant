/*
 * Configuration
 */

// If you enable deepsleep, you need to connect RST pin to GPIO16/D1, otherwise diconnect
#define DEEPSLEEP_ENABLED              true

// Important: If you enabled deepsleep, the serial output won't work
#define DEBUG_LEVEL                    1

// Generate a random id with:
// $ echo -n "sensor_"; head /dev/urandom | tr -dc A-Z0-9 | head -c 8; echo ""
#define SENSOR_NODE_ID                 "sensor_AAAABBBB"

#define WIFI_SSID                      ""
#define WIFI_PASSWORD                  ""

#define MQTT_CLIENTID                  SENSOR_NODE_ID
#define MQTT_SERVER                    ""

// Use the SHA1 fingerprint of the server certificate (NOT the CA certificate) in the following format:
#define MQTT_SERVER_TLS_FINGERPRINT    "XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX"

#define MQTT_PORT                      8883
#define MQTT_USERNAME                  SENSOR_NODE_ID
#define MQTT_PASSWORD                  ""

#define MQTT_CHANNEL_STATE             "/sensor/api/1/id/AAAABBBB/state/"

// Publish interval in seconds
#define PUBLISH_INTERVAL               30

#define OFFSET_TEMPERATURE             0
#define OFFSET_HUMIDITY                0

// Uncomment if on the board is an onboard LED
// Important: If you enabled deepsleep, the LED_BUILTIN on the NodeMCU won't work
#define PIN_STATUSLED                  LED_BUILTIN

#define PIN_DHT11                      D3
