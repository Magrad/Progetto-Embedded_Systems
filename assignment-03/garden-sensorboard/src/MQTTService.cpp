#include "MQTTService.h"

MQTTService::MQTTService() {

}

MQTTService::MQTTService(int led_pin, char* ssid, char* password) {
    client = PubSubClient(espClient);
    this->ssid = ssid;
    this->password = password;
}

void MQTTService::init() {    
    Serial.println(String("Connecting to ") + ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED) {
        delay(WAIT_TIMER);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void MQTTService::recconect() {
    while (!client.connected()) {
        Serial.println("Attempting MQTT connection...");

        String clientId = String("smart-garden-2122-client-") + String(random(0xffff), HEX);

        /* If the connection fails to be established it will
           wait 5 seconds before trying again */
        if(client.connect(clientId.c_str())) {
            Serial.println("connected");

            client.subscribe("smart-garden-2122");
        } else {
            Serial.println("failed, rc=");
            Serial.println(client.state());
            Serial.println(" try again in 5 seconds");

            delay(WAIT_TIMER * 10);
        }
    }
}

PubSubClient* MQTTService::getClient() {
    return &client;
}