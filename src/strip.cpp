#include "strip.h"

void arrayFunctions::reverseValues(int* array, int arraySize, int maxValue) {
    for (int i = 0; i < arraySize; i++) {
        array[i] = maxValue - array[i];
    }
}

void arrayFunctions::reverseValues(int* array, int arraySize) {
    reverseValues(array, arraySize, 255);
}

void arrayFunctions::getDifference(int* array1, int* array2, int* result, int arraySize) {
    for (int i = 0; i < arraySize; i++) {
        result[i] = array1[i] - array2[i];
    }
}

int* arrayFunctions::getDifference(int* array1, int* array2, int arraySize) {
    int* result = new int[arraySize];
    getDifference(array1, array2, result, arraySize);
    return result;
}

// we create this object so that we can use it in the rainbow function incase the user doesn't pass in a color object
CloudColoredLight* lastUsedColorObject = new CloudColoredLight();
/* PUBLIC FUNCTIONS */
LedStrip::LedStrip(uint8_t redPin, uint8_t greenPin, uint8_t bluePin, uint8_t whitePin) {
    this->redPin = redPin;
    this->greenPin = greenPin;
    this->bluePin = bluePin;
    this->whitePin = whitePin;
    LOG_INFO("Initializing LedStrip");
    SetupLedPins();
}

uint32_t LedStrip::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t white) {
    LOG("Creating color: " + String(red) + ", " + String(green) + ", " + String(blue) + ", " + String(white));
    return (uint32_t)red << 24 | (uint32_t)green << 16 | (uint32_t)blue << 8 | (uint32_t)white;
}

uint32_t LedStrip::Color(uint8_t red, uint8_t green, uint8_t blue) {
    return Color(red, green, blue, 0);
}

uint32_t LedStrip::Color(CloudColoredLight* color) {
    lastUsedColorObject = color;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t white;
    
    // calculate white using RBG, Saturation and Brightness.
    // Use the opposite of saturation, as 0 is white and 100 is color.
    white = ((100 - color->getSaturation()) / 100.0) * ((color->getBrightness()) / 100.0) * 255;
    color->getCloudValue().getRGB(red, green, blue);

    return Color(red, green, blue, white);
}

void LedStrip::update() {
    switch (currentMode) {
    case Modes::Normal:
        // do nothing, normal mode is handled by the onColorChange() function
        break;
    case Modes::Off:
        // do nothing, off mode is handled by the onModeChange() function
        break;
    case Modes::Rainbow:
        doRainbow();
        break;
    case Modes::Breathe:
        breathe();
        break;
    }
}

void LedStrip::SetupLedPins() {
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    pinMode(whitePin, OUTPUT);
}

void LedStrip::changeLedColor(uint32_t color) {
    smoothChangeTo(color); // call before setting new values, so that smooth transition can be made.
    this->color = color;
}

void LedStrip::changeLedColor(CloudColoredLight* color) {
    lastUsedColorObject = color;
    uint32_t color32 = Color(color);
    if (!color->getSwitch()) {
        color32 = Color(0,0,0,0);
    }
    changeLedColor(color32);
}

void LedStrip::changeMode(Modes mode) {
    currentMode = mode;
    switch (mode) {
    case Modes::Normal:
        Serial.println("Changing mode to Normal");
        changeLedColor(this->color);
        lastUsedColorObject->setSwitch(true);
        break;
    case Modes::Off:
        Serial.println("Changing mode to Off");
        changeLedColor(this->Color(0,0,0));
        lastUsedColorObject->setSwitch(false);
        break;
    }
}

void LedStrip::doRainbow() {
    doRainbow(lastUsedColorObject);
}

void LedStrip::doRainbow(CloudColoredLight* color) {
    doRainbow(color, 1000);
}

void LedStrip::doRainbow(CloudColoredLight* color, unsigned long delayTime) {
    if (currentMode != Modes::Rainbow) {
        Serial.println("Changing mode to Rainbow");
        changeMode(Modes::Rainbow);
    }

    rainbowHue = ((uint)color->getHue() + 8) % 360;

    color->setHue(rainbowHue);

    changeLedColor(color);
}

void LedStrip::breathe() {
    breathe(1000);
}

void LedStrip::breathe(unsigned long delayTime) {
    // We have to use smoothChangeTo instead of changeLedColor, because we want to use a different delayTime
    uint32_t oldColor = this->color;
    smoothChangeTo(0,0,0,0, delayTime); // turn off
    this->color = Color(0,0,0,0); // set color to 0,0,0,0 so that the smoothChangeTo function works properly
    smoothChangeTo(oldColor, delayTime); // turn back on
    this->color = oldColor; // set color back to original value
}

void LedStrip::setNightMode(bool active) {
    if (active) {
        Serial.println("Changing mode to Night");
        this->lastMode = this->currentMode;
        changeMode(Modes::Off);
    } else {
        Serial.println("Closing night mode");
        changeMode(this->lastMode != Modes::Off ? this->lastMode : Modes::Normal); // if last mode was off, then set to normal
    }
}


/* PRIVATE FUNCTIONS */
void LedStrip::smoothChangeTo(uint32_t color) {
    smoothChangeTo(color, 1000);
}

void LedStrip::smoothChangeTo(uint32_t color, unsigned long delayTime) {
    uint8_t red = (color >> 24) & 0xFF;
    uint8_t green = (color >> 16) & 0xFF;
    uint8_t blue = (color >> 8) & 0xFF;
    uint8_t white = color & 0xFF;

    smoothChangeTo(red, green, blue, white, delayTime);
}

void LedStrip::smoothChangeTo(uint8_t red, uint8_t green, uint8_t blue, uint8_t white) {
    smoothChangeTo(red, green, blue, white, 1000);
}

void LedStrip::smoothChangeTo(uint8_t red, uint8_t green, uint8_t blue, uint8_t white, unsigned long delayTime) {
    int newColors[4] = {red, green, blue, white};

    int oldColors[4] = {this->color >> 24 & 0xFF, this->color >> 16 & 0xFF,
        this->color >> 8 & 0xFF, this->color & 0xFF};
    
    smoothChangeTo(newColors, oldColors, delayTime);
}

void LedStrip::smoothChangeTo(int* newColors, int* oldColors, unsigned long delayTime) {
    int pins[4] = {redPin, greenPin, bluePin, whitePin};

    LOG("Changing colors to: " + String(newColors[0]) + ", " + String(newColors[1]) + ", " + String(newColors[2]) + ", " + String(newColors[3]));
    arrayFunctions::reverseValues(newColors, 4);
    arrayFunctions::reverseValues(oldColors, 4);

    int* differences = new int[4];
    arrayFunctions::getDifference(newColors, oldColors, differences, 4);

    int abs_max_difference = abs(*std::max_element(differences, differences + 4, 
        [](int a, int b) { return abs(a) < abs(b); }));

    LOG("Max difference: " + String(abs_max_difference));

    if (abs_max_difference == 0) {
        return;
    }

    for (int i = 0; i <= abs_max_difference; i++) {
        for (int j = 0; j < 4; j++) {
            int difference = differences[j];
            int newColor = oldColors[j];

            if (difference < 0) {
                newColor -= i;
                difference += 1;
            }
            if (difference > 0) {
                newColor += i;
                difference -= 1;
            }
            if (difference == 0) {
                newColor = newColors[j];
            }

            analogWrite(pins[j], newColor);
            differences[j] = difference;
        }
    delay(1000/abs_max_difference);
  }   
}

/**
 * returns an array of 4 integers, which are the differences between the current color and the new color.
*/
int* LedStrip::getAllDifferences(uint32_t color) {
    return getAllDifferences((color >> 24) & 0xFF, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
}

int* LedStrip::getAllDifferences(uint8_t red, uint8_t green, uint8_t blue, uint8_t white) {
    int* differences = new int[4];
    getAllDifferences(red, green, blue, white, differences);
    return differences;
}

void LedStrip::getAllDifferences(uint8_t red, uint8_t green, uint8_t blue, uint8_t white, int* differences) {
    int currentRed = (this->color >> 24) & 0xFF;
    int currentGreen = (this->color >> 16) & 0xFF;
    int currentBlue = (this->color >> 8) & 0xFF;
    int currentWhite = this->color & 0xFF;

    // cast to int to return negative values
    differences[0] = red - currentRed; 
    differences[1] = green - currentGreen;
    differences[2] = blue - currentBlue;
    differences[3] = white - currentWhite;
}
