#include "Color.h"

SimpleColor::SimpleColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t white) {
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->white = white;
};

SimpleColor::SimpleColor() {
    this->red = 0;
    this->green = 0;
    this->blue = 0;
    this->white = 0;
};

SimpleColor::SimpleColor(float hue, float sat, float bri) {
    
    ColorUtils::getRGBfromHSV(hue, sat, bri, this->red, this->green, this->blue);

    // calculate white using RBG, Saturation and Brightness.
    // Use the opposite of saturation, as 0 is white and 100 is color.
    this->white = ((100 - sat) / 100.0) * ((bri) / 100.0) * 255;
};
