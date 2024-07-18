#include "Color.h"

SimpleColor::SimpleColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t white) {
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->white = white;
}

SimpleColor::SimpleColor(float hue, float sat, float bri) {
    ColorUtils::getRGBfromHSV(hue, sat, bri, this->red, this->green, this->blue);

    // calculate white using RBG, Saturation and Brightness.
    // Use the opposite of saturation, as 0 is white and 100 is color.
    this->white = (100 - sat) / 100.0 * (bri / 100.0) * 255;
}

int SimpleColor::getHue() const {
    float hue, sat, bri;
    ColorUtils::getHSVfromRGB(this->red, this->green, this->blue, hue, sat, bri);
    return static_cast<int>(hue);
}

int SimpleColor::getSaturation() const {
    float hue, sat, bri;
    ColorUtils::getHSVfromRGB(this->red, this->green, this->blue, hue, sat, bri);
    return static_cast<int>(sat);
}

int SimpleColor::getBrightness() const {
    float hue, sat, bri;
    ColorUtils::getHSVfromRGB(this->red, this->green, this->blue, hue, sat, bri);
    return static_cast<int>(bri);
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
