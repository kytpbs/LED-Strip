#pragma once

#include <Arduino.h>

class SmoothPin {
    private:
        uint8_t pin;
        int currentValue;
        int targetValue;
        unsigned long lastChangeTime;
        unsigned long delayTime;
    public:
        SmoothPin(uint8_t pin);
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
