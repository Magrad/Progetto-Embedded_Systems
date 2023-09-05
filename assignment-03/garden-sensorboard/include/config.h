#ifndef __CONFIG_H__
#define __CONFIG_H__

#define LED_ALARM 32
#define PHOTORES_PIN 34
#define THERMO_PIN 35
#define FREQ 5000.0
#define ANALOG_ESP32 4096.0
#define ARDUINO_ANALOG 1024.0
#define DENOMINATOR 10.0
#define LM35_MIN -55
#define LM35_MAX 150
#define THERMO_MAX 4
#define PHOTORES_MAX 7

// Components Tasks
#define THERMO_TASK "ThermoTask"
#define PHOTORES_TASK "PhotoresTask"
#define READER_TASKS 0
#define WAIT_FOR_READ 1000L

// MQTT Task
#define MQTTSERVER_TASK "MQTTTask"
#define MQTT_TASK 1
#define SSID_WIFI "AndroidAP"
#define PASSWORD_WIFI "esiot-2122"
#define MQTT_SERVER "broker.mqtt-dashboard.com"
#define MQTT_PORT 1883
#define WAIT_TIMER 500
#define MSG_BUFFER_SIZE 50
#define MSG_TIMEOUT 5000L
#define ALARM "ALARM"
#define NOMINAL "NOMINAL"


#endif