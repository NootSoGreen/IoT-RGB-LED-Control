#include <Arduino.h>
#include "secrets.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"

// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"

// led is common anode (+)
//  LED pins
//        color GPIO PIN
#define RED_PIN 32
#define GREEN_PIN 33
#define BLUE_PIN 25

// led colour
int LED_RED = 0;
int LED_GREEN = 80;
int LED_BLUE = 0;

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(256);

void setColor(int red, int green, int blue)
{
    if (LED_RED != red || LED_GREEN != green || LED_BLUE != blue)
    {
        Serial.println("Setting Color to: ");
        Serial.print(red);
        Serial.print(", ");
        Serial.print(green);
        Serial.print(", ");
        Serial.print(blue);

        analogWrite(RED_PIN, 255 - red);
        analogWrite(GREEN_PIN, 255 - green);
        analogWrite(BLUE_PIN, 255 - blue);
        LED_RED = red;
        LED_GREEN = green;
        LED_BLUE = blue;
    }
}

void messageHandler(String &topic, String &payload)
{
    Serial.println();
    Serial.println("incoming: " + topic + " - " + payload);

    StaticJsonDocument<200> doc;
    deserializeJson(doc, payload);
    const int red = doc["red"];
    const int green = doc["green"];
    const int blue = doc["blue"];

    setColor(red, green, blue);
}

void connectAWS()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to Wi-Fi");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Configure WiFiClientSecure to use the AWS IoT device credentials
    net.setCACert(AWS_CERT_CA);
    net.setCertificate(AWS_CERT_CRT);
    net.setPrivateKey(AWS_CERT_PRIVATE);

    // Connect to the MQTT broker on the AWS endpoint we defined earlier
    client.begin(AWS_IOT_ENDPOINT, 8883, net);

    // Create a message handler
    client.onMessage(messageHandler);
    Serial.println();
    Serial.print("Connecting to AWS IOT");

    while (!client.connect(THINGNAME))
    {
        Serial.print(".");
        delay(100);
    }

    if (!client.connected())
    {
        Serial.println("AWS IoT Timeout!");
        return;
    }

    // Subscribe to a topic
    client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
    Serial.println();
    Serial.println("AWS IoT Connected!");
}

void publishMessage()
{
    StaticJsonDocument<200> doc;
    doc["time"] = millis();
    doc["hello"] = "hello world";
    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer); // print to client
    Serial.println("Hello1");
    client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void setup()
{
    Serial.begin(9600);
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    connectAWS();
    setColor(50, 150, 50);
}

void loop()
{
    // publishMessage();
    client.loop();
    delay(1000);
}