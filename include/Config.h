#pragma once

#define SERIAL_SPEED 115200

// UART0 IS ON PINS 0, 1, 12, 13, 16, 17

#define DHT22_PIN_DRYER 5
#define DHT22_PIN_INNER 6
#define DHT22_PIN_OUTER 7

#define FAN_PWM_PIN_DRYER   10
#define FAN_PWM_PIN_OUTER   11
#define FAN_PWM_PIN_PI      14
#define FAN_PWM_PIN_PRINTER 15

#define FAN_RPM_PIN_DRYER   18
#define FAN_RPM_PIN_OUTER   19
#define FAN_RPM_PIN_PI      20
#define FAN_RPM_PIN_PRINTER 21

#define HEATER_BUTTON_PIN_POWER 21
#define HEATER_BUTTON_PIN_PLUS 22
#define HEATER_BUTTON_PIN_MINUS 26
#define HEATER_BUTTON_PIN_MODE 27

#define HATCH_PIN 4