#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

#include "config.h"

#define FIRMWARE_VERSION  "1.0.0"
#define DHT_TYPE           DHT11

WiFiClientSecure secureWifiClient = WiFiClientSecure();
PubSubClient mqttClient = PubSubClient(secureWifiClient, MQTT_SERVER_TLS_FINGERPRINT);
const int BUFFER_SIZE = JSON_OBJECT_SIZE(20);
DHT dhtSensor = DHT(PIN_DHT11, DHT_TYPE);

unsigned long lastPublishUpdate = 0;

/*
 * Setup
 */

void setup()
{
    #if DEEPSLEEP_ENABLED == false
        Serial.begin(115200);
        delay(250);

        char buffer[64] = {0};
        sprintf(buffer, "setup(): The node '%s' was powered up.", MQTT_CLIENTID);
        Serial.println();
        Serial.println(buffer);
    #endif

    #ifdef PIN_STATUSLED
        pinMode(PIN_STATUSLED, OUTPUT);
    #endif

    setupWifi();
    setupSensor();
    setupMQTT();

    // If deep sleep is enabled, the loop() will never reached, thus do everything here
    #if DEEPSLEEP_ENABLED == true
        connectMQTT();
        readValuesAndPublishState();
        startDeepSleep();
    #endif
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
    Serial.println("setupSensor(): Setup DHT sensor...");
    dhtSensor.begin();
}

void setupMQTT()
{
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
}

void loop()
{
    connectMQTT();
    mqttClient.loop();

    if ((micros() - lastPublishUpdate) > (PUBLISH_INTERVAL * 1000000))
    {
        readValuesAndPublishState();
        lastPublishUpdate = micros();
    }
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
        }
        else
        {
            Serial.printf("connectMQTT(): Connection failed with error code %i. Try again...\n", mqttClient.state());
            blinkStatusLED(3);
            delay(500);
        }
    }
}

void readValuesAndPublishState()
{
    const float humidityValue = dhtSensor.readHumidity();
    const float temperatureValue = dhtSensor.readTemperature();

    if (isnan(humidityValue) || isnan(temperatureValue))
    {
        Serial.println("readValuesAndPublishState(): The humidity and temperature values could not be read!");
    }
    else
    {
        #if DEBUG_LEVEL >= 1
            Serial.println("readValuesAndPublishState(): The humidity and temperature values was read successfully.");
            Serial.print(F("readValuesAndPublishState():"));
            Serial.print(F(" humidityValue = "));
            Serial.print(humidityValue);
            Serial.print(F(", temperatureValue = "));
            Serial.print(temperatureValue);
            Serial.println();
        #endif

        StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();

        // String cast seems needed
        root["humidity"] = (String) humidityValue;
        root["temperature"] = (String) temperatureValue;

        char payloadMessage[root.measureLength() + 1];
        root.printTo(payloadMessage, sizeof(payloadMessage));

        #if DEBUG_LEVEL >= 1
            Serial.printf("readValuesAndPublishState(): Publish message on channel '%s': %s\n", MQTT_CHANNEL_STATE, payloadMessage);
        #endif

        mqttClient.publish(MQTT_CHANNEL_STATE, payloadMessage, true);
    }
}

void startDeepSleep()
{
    Serial.println("startDeepSleep(): Disconnecting MQTT connection...");
    mqttClient.disconnect();

    Serial.println("startDeepSleep(): Disconnecting Wifi connection...");
    WiFi.disconnect();

    Serial.println("startDeepSleep(): Shutting down. Going to deep sleep...");
    ESP.deepSleep(PUBLISH_INTERVAL * 1000000, WAKE_RF_DEFAULT);

    Serial.println("startDeepSleep(): Deep sleep failed!");

    while(true)
    {
        blinkStatusLED(5);
        delay(5000);
    }
}
