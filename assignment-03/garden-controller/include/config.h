#ifndef __CONFIG_H__
#define __CONFIG_H__

#define SECONDS 1000L
#define MINUTES 60L*SECONDS

// Led
#define ON_OFF_LEDS 2
#define BRIGHTNESS_LEDS 2
#define N_LEDS 4
#define LED_1 5
#define LED_2 6
#define LED_3 9
#define LED_4 10
#define LED_INTERVAL 4
#define MAX_BRIGHTNESS 255
#define LED_ON 1
#define LED_OFF 0

// Servo
#define SERVO_PIN 11
#define FULL_SWEEP 180
#define SERVO_INTERVAL 4
#define MAX_SPEED 25
#define N_SERVO 1

// Components
#define N_COMPONENTS N_LEDS + N_SERVO


// Bluetooth
#define DEFAULT_RX_PIN 2
#define DEFAULT_TX_PIN 3
#define RX_PIN 12
#define TX_PIN 13

// State of day
#define DAY 0
#define AFTERNOON 1
#define EVENING 2

// Irrigation System
#define ACTIVITY_SEC 10L
#define SLEEP_MIN 30L
#define ALARM_TEMPERATURE 5

#endif