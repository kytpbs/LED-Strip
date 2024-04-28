#pragma once

#ifdef ARDUINO_ARCH_ESP32
#define GREEN_PIN 19
#define RED_PIN 18
#define BLUE_PIN 5
#define WHITE_PIN 17
#elif defined(ARDUINO_ARCH_ESP8266)
#define GREEN_PIN 16
#define RED_PIN 5
#define BLUE_PIN 4
#define WHITE_PIN 0
#else
#error "This library only supports ESP32 and ESP8266"
#endif
