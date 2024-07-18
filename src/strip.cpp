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
    rainbowHue = color.getHue();
}

void LedStrip::instantFillColor(SimpleColor color) {
    instantChangeTo(color);
    this->currentColor = color;
    rainbowHue = color.getHue();
}

void LedStrip::changeModeTo(Modes mode) {
    currentMode = mode;
    // if we are in night mode, then nightModeChange can't update this.
    // meaning the user is changing the mode.
    if (mode != Modes::Off && !this->isNightMode()) {
        this->lastMode = mode;
    }
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
        switchToBreathe();
        break;
    case Modes::Rainbow:
        switchToRainbow();
        break;
    case Modes::Blink:
        switchToBlink();
        break;
    }
}

void LedStrip::setNightMode(bool active) {
    if (active) {
        Serial.println("Changing mode to Night");
        this->lastMode = this->currentMode;
        changeModeTo(Modes::Off);
        this->nightModeActive = true;
    } else {
        Serial.println("Closing night mode");
        this->nightModeActive = false;
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

void LedStrip::instantChangeTo(SimpleColor color) {
    this->redPin.instantWrite(255 - color.red);
    this->greenPin.instantWrite(255 - color.green);
    this->bluePin.instantWrite(255 - color.blue);
    this->whitePin.instantWrite(255 - color.white);
}

void LedStrip::switchToRainbow() {
    changeCallListTo(LEDCommand([=]() {
        if (this->isChanging()) {
            return;
        }
        const SimpleColor color = SimpleColor(this->rainbowHue, 100, 100);
        this->smoothChangeTo(color, 125);
        this->rainbowHue += 1;
        this->rainbowHue %= 360;
    },true, "Rainbow"));
}

void LedStrip::switchToBlink() {
    isON = false;
    changeCallListTo(LEDCommand([=]() {
        if (millis() - lastBlinkTime < 250) {
            return;
        }

        lastBlinkTime = millis();
        
        if (isON) {
            this->instantChangeTo(SimpleColor());
        } else {
            this->instantChangeTo(this->currentColor);
        }

        isON = !isON;
    },true, "Blink"));

}

void LedStrip::switchToBreathe() {
    isON = false;
    changeCallListTo(LEDCommand([=]() {
        if (this->isChanging()) {
            return;
        }
        
        if (isON) {
            this->smoothChangeTo(SimpleColor(), 1500);
        } else {
            this->smoothChangeTo(this->currentColor, 1500);
        }
        isON = !isON;
    },true, "Breathe"));
}

void LedStrip::switchToRandomColorSwitch() {
    changeCallListTo(LEDCommand([=]() {
        if (millis() - lastBlinkTime < 250) {
            return;
        }
        
        lastBlinkTime = millis();

        const SimpleColor color = SimpleColor(random(0, 360), 100, 100);
        this->smoothChangeTo(color, 500);
    },true, "RandomColorSwitch"));
}

void LedStrip::addToCallList(const LEDCommand& command) {
    this->callList.push_back(command);
}

void LedStrip::changeCallListTo(const LEDCommand& command) {
    this->callList.clear();
    this->callList.push_back(command);
}

bool LedStrip::isChanging() {
    return this->redPin.isChanging() || this->greenPin.isChanging() || this->bluePin.isChanging() || this->whitePin.isChanging();
}
