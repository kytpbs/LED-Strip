#pragma once
#include <Arduino.h>
#include "ColorUtils.h"

class SimpleColor {
public:
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t white;

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
     * All values are set to 0, which is black
    */
    SimpleColor();
    /**
     * @brief Construct a new SimpleColor object
     * @param hue Hue of hsv color (0-360)
     * @param sat Saturation of hsv color (0-100)
     * @param bri Brightness of hsv color (0-100)
    */
    SimpleColor(float hue, float sat, float bri);
};