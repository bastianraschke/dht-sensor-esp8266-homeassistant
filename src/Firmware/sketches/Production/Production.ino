#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

#include "config.h"

#define FIRMWARE_VERSION  "1.0.0"

WiFiClientSecure secureWifiClient = WiFiClientSecure();
PubSubClient mqttClient = PubSubClient(secureWifiClient, MQTT_SERVER_TLS_FINGERPRINT);
const int BUFFER_SIZE = JSON_OBJECT_SIZE(20);

/*
 * Setup
 */

void setup()
{
    Serial.begin(115200);
    delay(250);

    char buffer[64] = {0};
    sprintf(buffer, "setup(): The node '%s' was powered up.", MQTT_CLIENTID);
    Serial.println();
    Serial.println(buffer);

    #ifdef PIN_STATUSLED
        pinMode(PIN_STATUSLED, OUTPUT);
    #endif

    setupWifi();
    setupSensor();
    setupMQTT();
}

void setupWifi()
{
    Serial.printf("setupWifi(): Connecting to to Wi-Fi access point '%s'...\n", WIFI_SSID);

    // Do not store Wi-Fi config in SDK flash area
    WiFi.persistent(false);

    // Disable auto Wi-Fi access point mode
    WiFi.mode(WIFI_STA);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        // Blink 2 times when connecting
        blinkStatusLED(2);

        delay(500);
        Serial.println(F("setupWifi(): Connecting..."));
    }

    Serial.print(F("setupWifi(): Connected to Wi-Fi access point. Obtained IP address: "));
    Serial.println(WiFi.localIP());
}

void blinkStatusLED(const int times)
{
    #ifdef PIN_STATUSLED
        for (int i = 0; i < times; i++)
        {
            // Enable LED
            digitalWrite(PIN_STATUSLED, LOW);
            delay(100);

            // Disable LED
            digitalWrite(PIN_STATUSLED, HIGH);
            delay(100);
        }
    #endif
}

void setupSensor()
{
    Serial.println("setupSensor(): Setup DHT11...");
}

void setupMQTT()
{
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
}

void publishState()
{
    StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();

    // root["state"] = (stateOnOff == true) ? "ON" : "OFF";

    // JsonObject& color = root.createNestedObject("color");
    // color["r"] = originalRedValue;
    // color["g"] = originalGreenValue;
    // color["b"] = originalBlueValue;

    // if (LED_TYPE == RGBW)
    // {
    //     root["white_value"] = originalWhiteValue;
    // }

    // root["brightness"] = brightness;

    // char payloadMessage[root.measureLength() + 1];
    // root.printTo(payloadMessage, sizeof(payloadMessage));

    // Serial.printf("publishState(): Publish message on channel '%s': %s\n", MQTT_CHANNEL_STATE, payloadMessage);
    // mqttClient.publish(MQTT_CHANNEL_STATE, payloadMessage, true);
}

void loop()
{
    connectMQTT();
    mqttClient.loop();
}

void connectMQTT()
{
    if (mqttClient.connected() == true)
    {
        return ;
    }

    Serial.printf("connectMQTT(): Connecting to MQTT broker '%s:%i'...\n", MQTT_SERVER, MQTT_PORT);

    while (mqttClient.connected() == false)
    {
        Serial.println("connectMQTT(): Connecting...");

        if (mqttClient.connect(MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD) == true)
        {
            Serial.println("connectMQTT(): Connected to MQTT broker.");

            // (Re)subscribe on topics
            mqttClient.subscribe(MQTT_CHANNEL_COMMAND);

            // Initially publish current state
            publishState();
        }
        else
        {
            Serial.printf("connectMQTT(): Connection failed with error code %i. Try again...\n", mqttClient.state());
            blinkStatusLED(3);
            delay(500);
        }
    }
}
