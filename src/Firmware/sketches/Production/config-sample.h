#include <DHT.h>

/*
 * Configuration
 */

/*
 * If you want to enable deepsleep, you need to:
 * - connect RST pin to GPIO16/D1, otherwise diconnect
 * - set DEBUG_LEVEL to 0, otherwise the device keeps resetting
 */
#define DEEPSLEEP_ENABLED              false

#define DEBUG_LEVEL                    1

// Generate a random id with:
// $ echo -n "sensor_"; head /dev/urandom | tr -dc A-Z0-9 | head -c 8; echo ""
#define SENSOR_NODE_ID                 "sensor_AAAABBBB"

#define WIFI_SSID                      ""
#define WIFI_PASSWORD                  ""

#define MQTT_CLIENTID                  SENSOR_NODE_ID

// Put the host/IPv4 address of your MQTT broker here
#define MQTT_SERVER                    ""

// Use the SHA1 fingerprint of the server certificate (NOT the CA certificate) in the following format:
#define MQTT_SERVER_TLS_FINGERPRINT    "XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX"

#define MQTT_PORT                      8883
#define MQTT_USERNAME                  SENSOR_NODE_ID
#define MQTT_PASSWORD                  ""

#define MQTT_CHANNEL_STATE             "/sensor/api/1/id/AAAABBBB/state/"

// Publish interval in seconds
#define PUBLISH_INTERVAL               30

// Define some offsets to correct measurements
#define OFFSET_TEMPERATURE             0
#define OFFSET_HUMIDITY                0

// Set indication LED to a value >= 0 to enable it
// Important: If you enabled deepsleep, the LED_BUILTIN on the NodeMCU won't work
#define PIN_STATUSLED                  LED_BUILTIN

#define PIN_DHT_SENSOR                 D3
#define DHT_SENSOR_TYPE                DHT11
