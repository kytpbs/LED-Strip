#include "ColorUtils.h"

void ColorUtils::getRGBfromHSV(float hue, float sat, float bri, uint8_t& r, uint8_t& g, uint8_t& b) {
    float fC = (bri / 100) * (sat / 100);
    float fHPrime = fmod(hue / 60.0, 6);
    float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
    float fM = (bri / 100) - fC;
    float fR, fG, fB;

    if (0 <= fHPrime && fHPrime < 1) {
      fR = fC;
      fG = fX;
      fB = 0;
    } else if (1 <= fHPrime && fHPrime < 2) {
      fR = fX;
      fG = fC;
      fB = 0;
    } else if (2 <= fHPrime && fHPrime < 3) {
      fR = 0;
      fG = fC;
      fB = fX;
    } else if (3 <= fHPrime && fHPrime < 4) {
      fR = 0;
      fG = fX;
      fB = fC;
    } else if (4 <= fHPrime && fHPrime < 5) {
      fR = fX;
      fG = 0;
      fB = fC;
    } else if (5 <= fHPrime && fHPrime < 6) {
      fR = fC;
      fG = 0;
      fB = fX;
    } else {
      fR = 0;
      fG = 0;
      fB = 0;
    }
    r = lrint((fR + fM) * 255);
    g = lrint((fG + fM) * 255);
    b = lrint((fB + fM) * 255);
}

int ColorUtils::getMaxElement(int* arr, int size) {
    int max = abs(arr[0]);
    for (int i = 0; i < size; i++) {
        if (abs(arr[i]) > max) {
            max = abs(arr[i]);
        }
    }
    return max;
}