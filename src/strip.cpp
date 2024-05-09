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
        break;
    case Modes::Off:
        fillColor(SimpleColor());
        break;
    case Modes::Breathe:
        switchToRainbow();
        break;
    case Modes::Rainbow:
        //TODO: implement this
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


/* PRIVATE FUNCTIONS */
void LedStrip::smoothChangeTo(SimpleColor color, unsigned long delayTime) {
    this->redPin.write(color.red, delayTime);
    this->greenPin.write(color.green, delayTime);
    this->bluePin.write(color.blue, delayTime);
    this->whitePin.write(color.white, delayTime);
}

void LedStrip::smoothChangeTo(SimpleColor color) {
    smoothChangeTo(color, 1000);
}

void LedStrip::switchToRainbow() {
    addToCallList(LEDCommand([=](void) {
        SimpleColor color = SimpleColor(this->rainbowHue, 100, 100);
        fillColor(color);
        this->rainbowHue += 8;
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
