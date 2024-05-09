#include "strip.h"

/* PUBLIC FUNCTIONS */
LedStrip::LedStrip(uint8_t redPin, uint8_t greenPin, uint8_t bluePin, uint8_t whitePin) {
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    pinMode(whitePin, OUTPUT);

    this->redPin = SmoothPin(redPin);
    this->greenPin = SmoothPin(greenPin);
    this->bluePin = SmoothPin(bluePin);
    this->whitePin = SmoothPin(whitePin);
}

void LedStrip::update() {
    this->redPin.update();
    this->greenPin.update();
    this->bluePin.update();
    this->whitePin.update();

    if (this->callList.empty()) {
        return;
    }

    auto command = this->callList.front();
    command.execute();
    if (command.hasEnded()) {
        this->callList.erase(this->callList.begin());
    }
}

void LedStrip::fillColor(SimpleColor color) {
    smoothChangeTo(color);
    this->currentColor = color;
}

void LedStrip::changeModeTo(Modes mode) {
    currentMode = mode;
    switch (mode) {
    case Modes::Normal:
        fillColor(this->currentColor);
        callList.clear();
        break;
    case Modes::Off:
        smoothChangeTo(SimpleColor());
        callList.clear();
        break;
    case Modes::Breathe:
        //TODO: implement this
        break;
    case Modes::Rainbow:
        switchToRainbow();
        break;
    }
}

void LedStrip::setNightMode(bool active) {
    if (active) {
        Serial.println("Changing mode to Night");
        this->lastMode = this->currentMode;
        changeModeTo(Modes::Off);
    } else {
        Serial.println("Closing night mode");
        changeModeTo(this->lastMode != Modes::Off ? this->lastMode : Modes::Normal); // if last mode was off, then set to normal
    }
}

SimpleColor LedStrip::getCurrentColor() {
    return SimpleColor(this->redPin.getTargetValue(), this->greenPin.getTargetValue(), this->bluePin.getTargetValue(), this->whitePin.getTargetValue());
}

void LedStrip::getRGB(uint8_t &r, uint8_t &g, uint8_t &b) {
    r = 255 - this->redPin.getTargetValue();
    g = 255 - this->greenPin.getTargetValue();
    b = 255 - this->bluePin.getTargetValue();
}

/* PRIVATE FUNCTIONS */
void LedStrip::smoothChangeTo(SimpleColor color, unsigned long delayTime) {
    this->redPin.write(255 - color.red, delayTime);
    this->greenPin.write(255 - color.green, delayTime);
    this->bluePin.write(255 - color.blue, delayTime);
    this->whitePin.write(255 - color.white, delayTime);
}

void LedStrip::smoothChangeTo(SimpleColor color) {
    smoothChangeTo(color, 1000);
}

void LedStrip::switchToRainbow() {
    changeCallListTo(LEDCommand([=](void) {
        if (this->isChanging()) {
            return;
        }
        SimpleColor color = SimpleColor(this->rainbowHue, 100, 100);
        this->smoothChangeTo(color, 125);
        this->rainbowHue += 1;
        this->rainbowHue %= 360;
    },true, "Rainbow"));
}

void LedStrip::addToCallList(LEDCommand command) {
    this->callList.push_back(command);
}

void LedStrip::changeCallListTo(LEDCommand command) {
    this->callList.clear();
    this->callList.push_back(command);
}

bool LedStrip::isChanging() {
    return this->redPin.isChanging() || this->greenPin.isChanging() || this->bluePin.isChanging() || this->whitePin.isChanging();
}
