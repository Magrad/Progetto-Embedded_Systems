#ifndef __CONFIG__
#define __CONFIG__

/* Hardware */
#define N_BUTTONS 3
#define BUT01_PIN 2
#define BUT02_PIN 3
#define BUT03_PIN 4

#define PIR_PIN 5

#define TRIG_PIN 7
#define ECHO_PIN 8

#define SERVO_PIN 9
#define FULL_SWEEP 180
#define FULL_SWEEP_COMPLETED 0

#define DIGITAL_THERMO_PIN 10
#define ANALOG_THERMO_PIN A1
#define DHTTYPE DHT11
#define TEMP_MIN 17
#define TEMP_MAX 24

#define POT_PIN A0

#define LCD_SDA A4
#define LCD_SCL A5
#define I2C_ADDRESS 0x27
#define LCD_COLUMNS 20
#define LCD_ROWS 4

/* Software */
#define SECONDS 1000l
#define ONE_EIGHTH 125l
#define T_IDLE 60l
#define T_USER_IDLE 5l
#define T_CHECK 180l
#define T_MAKING 10L
#define T_TIMEOUT 5l

/* Welcome */
#define N_MAX 3
#define WELCOME_MSG_1 "Welcome"
#define WELCOME_MSG_2 "enjoy your drink"
#define READY_MSG "Ready."
#define ASLEEP_MSG "Asleep"
/* User */
#define COFFEE_MSG "Coffee"
#define TEA_MSG "Tea"
#define CHOCOLATE_MSG "Chocolate"
/* Making */
#define MAKING_MSG "Making a "
#define THE_MSG "The "
#define DRINK_READY_MSG " is ready."
#define SELF_TEST_MSG "Self Test"
/* Assistance */
#define ASSISTANCE_MODALITY_MSG "Assistance Required"
/* MsgService */
#define FILL "Fill "
#define RESTART "Restart "
#define N_TESTS "Test "
#define STATE "State "
#define IDLE_MSG "Idle"
#define WORKING_MSG "Working"
#define ASSISTANCE_MSG "Assistance"

#endif
