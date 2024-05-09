#pragma once

#include <Arduino.h>

class SmoothPin {
    private:
        uint8_t pin;
        int currentValue;
        int targetValue;
        unsigned long lastChangeTime;
        unsigned long delayBetweenTime;
    public:
        SmoothPin() = default;
        SmoothPin(uint8_t pin);
        /**
         * @brief Write value to pin.
         * gradually changes to value every time update() is called without delay.
         * 
         * @note use SmoothPin::instantWrite() if you want to change value instantly
         * 
         * @param value value to write
        */
        void write(int value);
        /**
         * @brief Write value to pin instantly instead of changing gradually
         * 
         * @param value value to write
        */
        void instantWrite(int value);
        /**
         * @brief Write value to pin with delay
         * @param value value to write
        */
        void write(int value, unsigned int totalDelayMillis);
        void update();
};
