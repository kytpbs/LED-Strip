#include "strip.h"

/* PUBLIC FUNCTIONS */
LedStrip::LedStrip(uint8_t redPin, uint8_t greenPin, uint8_t bluePin, uint8_t whitePin) {
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    pinMode(whitePin, OUTPUT);

    this->redPin = redPin;
    this->greenPin = greenPin;
    this->bluePin = bluePin;
    this->whitePin = whitePin;
}


void LedStrip::update() {
    if (this->callList.empty()) {
        return;
    }

    auto command = this->callList.front();
    command.execute();
    if (command.hasEnded()) {
        this->callList.erase(this->callList.begin());
    }
}

void LedStrip::addToCallList(LEDCommand command) {
    this->callList.push_back(command);
}

void LedStrip::changeCallListTo(LEDCommand command) {
    this->callList.clear();
    this->callList.push_back(command);
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
    int differences[4] = {abs(color.red - this->currentColor.red), abs(color.green - this->currentColor.green),
        abs(color.blue - this->currentColor.blue), abs(color.white - this->currentColor.white)};
    int maxDifference = ColorUtils::getMaxElement(differences, 4);

    if (maxDifference <= 0) {
        return;
    }

    for (int i = 0; i <= maxDifference; i++) {
        // change red
        int redDifference = color.red - this->currentColor.red;
        int redValue = redDifference > 0 ? this->currentColor.red + i : this->currentColor.red - i;
        this->currentColor.red = this->currentColor.red + i;
        addToCallList(LEDCommand([=](void) {
            analogWrite(redPin, redValue);
        },false,"smooth change red"));

        // change green
        int greenDifference = color.green - this->currentColor.green;
        int greenValue = greenDifference > 0 ? this->currentColor.green + i : this->currentColor.green - i;
        this->currentColor.green = this->currentColor.green + i;
        addToCallList(LEDCommand([=](void) {
            analogWrite(greenPin, greenValue);
        },false,"smooth change blue"));

        // change blue
        int blueDifference = color.blue - this->currentColor.blue;
        int blueValue = blueDifference > 0 ? this->currentColor.blue + i : this->currentColor.blue - i;
        this->currentColor.blue = this->currentColor.blue + i;
        addToCallList(LEDCommand([=](void) {
            analogWrite(bluePin, blueValue);
        },false,"smooth change green"));

        // change white
        int whiteDifference = color.white - this->currentColor.white;
        int whiteValue = whiteDifference > 0 ? this->currentColor.white + i : this->currentColor.white - i;
        this->currentColor.white = this->currentColor.white + i;
        addToCallList(LEDCommand([=](void) {
            analogWrite(whitePin, whiteValue);
        },false,"smooth change white"));

        // delay
        addToCallList(LEDCommand([=](void) {
            delay(delayTime/maxDifference);
        },false,"smooth change delay"));
    }
}

void LedStrip::smoothChangeTo(SimpleColor color) {
    smoothChangeTo(color, 1000);
}

void LedStrip::switchToRainbow() {
    addToCallList(LEDCommand([=](void) {
        SimpleColor color = ColorUtils::getColorFromHSV(this->rainbowHue, 100, 100);
        fillColor(color);
        this->rainbowHue += 8;
        this->rainbowHue %= 360;
        // we call ourselves again in the end so we do not disturb the fill commands...
        // should have fixed this in the implementation, but this is a quick fix hopefully, will fix later
        // this is not a recursive function, but will keep get called by the update function until the mode is changed
        this->switchToRainbow();
    },false, "Rainbow"));
}