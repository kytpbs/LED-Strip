#pragma once
#include <Arduino.h>
#include "Color.h"
namespace ColorUtils {
    /**
     * @brief Convert RGB to HSV 
     * @cite directly copied from ArduinoCloud "CloudColor.h" file
     * @param r the pointer to the red value
     * @param g the pointer to the green value
     * @param b the pointer to the blue value
     * @param hue the hue
     * @param sat the saturation
     * @param bri the brightness/value
    */
    void getRGBfromHSV(float hue, float sat, float bri, uint8_t *r, uint8_t *g, uint8_t *b);

    SimpleColor getColorFromHSV(float hue, float sat, float bri);
    int getMaxElement(int* arr, int size);
}