#include "SmoothPin.h"

SmoothPin::SmoothPin(uint8_t pin) {
    this->pin = pin;
    pinMode(pin, OUTPUT);
    this->currentValue = 0;
    this->targetValue = 0;
    this->lastChangeTime = 0;
    this->delayBetweenTime = 0;
}

void SmoothPin::instantWrite(int value) {
    this->currentValue = value;
    this->targetValue = value;
    analogWrite(this->pin, this->currentValue);
}

void SmoothPin::write(int value, unsigned int totalDelayMillis) {
    this->targetValue = value;
    this->lastChangeTime = 0;
    unsigned int diff = abs(this->currentValue - value);
    if (diff == 0) {
        return;
    }
    this->delayBetweenTime = totalDelayMillis / diff;
}

void SmoothPin::write(int value) {
    this->write(value, 0);
}

void SmoothPin::update() {
    if (this->currentValue == this->targetValue) {
        return;
    }

    unsigned long currentTime = millis();
    if (currentTime - this->lastChangeTime > this->delayBetweenTime) {
        if (this->currentValue < this->targetValue) {
            this->currentValue++;
        } else {
            this->currentValue--;
        }
        analogWrite(this->pin, this->currentValue);
        this->lastChangeTime = currentTime;
    }
}
