#pragma once
#include <Arduino.h>
#include "ColorUtils.h"

class SimpleColor {
public:
    // Hold onto both RGB and HSV values this way we can convert without recalculating,
    // and also we reduce the side effects of converting between them

    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
    uint8_t white = 0;

    uint16_t hue = 0;
    uint8_t sat = 0;
    uint8_t bri = 0;

    /**
     * @brief Construct a new SimpleColor object
     * All values are set to 0, which is black
    */
    SimpleColor() = default;

    /**
     * @brief Construct a new SimpleColor object
     * @param red Red of rgbw color
     * @param green Green of rgbw color
     * @param blue Blue of rgbw color
     * @param white White of rgbw color
    */
    SimpleColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t white);

    /**
     * @brief Construct a new SimpleColor object
     * @param hue Hue of hsv color (0-360)
     * @param sat Saturation of hsv color (0-100)
     * @param bri Brightness of hsv color (0-100)
    */
    SimpleColor(float hue, float sat, float bri);

    int getHue() const;
    int getSaturation() const;
    int getBrightness() const;

    /**
     * Returns the string in a hex format
     */
    String toHexRGB() const;
};