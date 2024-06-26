#pragma once

#include <Arduino.h>

class SmoothPin {
    private:
        uint8_t pin = -1; // this is so, if for some reason the pin is not set, it will not write to random pins
        int currentValue = 0;
        int targetValue = 0;
        unsigned long lastChangeTime = 0;
        unsigned long delayBetweenTime = 0;
    public:
        SmoothPin() = default;
        explicit SmoothPin(uint8_t pin);
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
        /**
         * @brief Get current value of pin
         * 
         * @return current value of pin
         * 
         * @note this value is updated with every call to update()
         * so it can be used to sync with other variables.
        */
        int getCurrentValue() const { return this->currentValue; }
        int getTargetValue() const { return this->targetValue; }
        bool isChanging() const { return this->currentValue != this->targetValue; };
};
