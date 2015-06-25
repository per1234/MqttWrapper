// #define DEBUG_MODE
// #define DEBUG_LEVEL_VERBOSE

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <MqttWrapper.h>
#include <WiFiHelper.h>
#include <PubSubClient.h>
#include "mqtt_configuration.h"

const char* ssid     = "CMMC.32";
const char* pass     = "guestnetwork";

MqttWrapper *mqtt;
WiFiHelper *wifi;


void hook_prepare_data(JsonObject** root)
{
    JsonObject& data = (*root)->at("d");

    data["myName"] = "NAT";
    data["adc"] = analogRead(A0);;

}

void hook_publish_data(char* data)
{
    Serial.print("PUBLISH: ->");
    Serial.println(data);
}


void init_wifi()
{
    wifi = new WiFiHelper(ssid, pass);

    wifi->on_connecting([](const char* message)
    {
        Serial.println("connecting...");  
    });

    wifi->on_connected([](const char* message)
    {
        Serial.println("");
        Serial.println("WiFi connected");  
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    });

    wifi->connect();
}

void init_mqtt()
{
    String organization = String("r6crrd");
    String url = organization + ".messaging.internetofthings.ibmcloud.com";

    mqtt = new MqttWrapper(url.c_str()  , 1883, hook_configuration);
    mqtt->set_prepare_data_hook(hook_prepare_data, 2000);
    mqtt->set_publish_data_hook(hook_publish_data);
    mqtt->connect();    
}

void init_hardware()
{
    Serial.begin(115200);
    pinMode(0, INPUT_PULLUP);
    delay(10);
    Serial.println();
    Serial.println();
    Serial.println("START..");
}

void setup()
{
    init_hardware();
    init_wifi();
    init_mqtt();
}

void loop()
{
    wifi->loop();
    mqtt->loop();
}