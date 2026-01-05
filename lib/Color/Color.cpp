#include "Color.h"

SimpleColor::SimpleColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t white) {
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->white = white;

    float hue, sat, bri;
    ColorUtils::getHSVfromRGB(red, green, blue, hue, sat, bri);

    this->hue = static_cast<uint16_t>(hue);
    this->sat = static_cast<uint8_t>(sat);
    this->bri = static_cast<uint8_t>(bri);
}

SimpleColor::SimpleColor(float hue, float sat, float bri) {
    // calculate RGB using HSV
    ColorUtils::getRGBfromHSV(hue, sat, bri, this->red, this->green, this->blue);

    this->hue = static_cast<uint16_t>(hue);
    this->sat = static_cast<uint8_t>(sat);
    this->bri = static_cast<uint8_t>(bri);

    // calculate white using RBG, Saturation and Brightness.
    // Use the opposite of saturation, as 0 is white and 100 is color.
    this->white = (100 - sat) / 100.0 * (bri / 100.0) * 255;
}

int SimpleColor::getHue() const {
    return this->hue;
}

int SimpleColor::getSaturation() const {
    return this->sat;
}

int SimpleColor::getBrightness() const {
    return this->bri;
}

String turnIntToHex(int value) {
    String hex = String(value, HEX);
    if (hex.length() == 1) {
        hex = "0" + hex;
    }
    return hex;
}

String SimpleColor::toHexRGB() const {
    return "#" + turnIntToHex(this->red) + turnIntToHex(this->green) + turnIntToHex(this->blue);
}
