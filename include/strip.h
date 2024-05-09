#pragma once
#include <list>
#include <Color.h>
#include "LEDCommand.h"
#include "SmoothPin.h"

enum Modes {
  Normal,
  Breathe,
  Rainbow,
  Off
};


class LedStrip {
    private:
        Modes currentMode;
        Modes lastMode; // used for night mode
        bool nightModeActive;
        uint16_t rainbowHue; // can't do uint8_t because it will overflow
        SimpleColor currentColor;

        SmoothPin redPin;
        SmoothPin greenPin;
        SmoothPin bluePin;
        SmoothPin whitePin;

        std::list<LEDCommand> callList;

        void smoothChangeTo(SimpleColor color, unsigned long delayTime);
        void smoothChangeTo(SimpleColor color);

        void switchToRainbow();
        void switchToBreathe();

        void addToCallList(LEDCommand command);
        void changeCallListTo(LEDCommand command);
    public:
        LedStrip() = default;
        LedStrip(uint8_t redPin, uint8_t greenPin, uint8_t bluePin, uint8_t whitePin);
        /**
         * @brief Update led strip, must be called in loop repeatedly.
         * @warning Nothing will work if this function is not called in loop
        */
        void update();

        /**
         * @brief Set night mode, this will turn off the led strip until night mode is turned off.
         * @param active true if night mode is active, false otherwise
        */
        void setNightMode(bool active);

        /**
         * @brief Change color of led strip
         * @param color color to change to
        */
        void fillColor(SimpleColor color);

        /**
         * @brief Change mode of led strip
         * @param mode mode to change to
        */
        void changeModeTo(Modes mode);

        Modes getCurrentMode() {
            return currentMode;
        };

        SimpleColor getCurrentColor();
        void getRGB(uint8_t &r, uint8_t &g, uint8_t &b);
};
