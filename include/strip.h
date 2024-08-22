#pragma once
#include <list>
#include <Color.h>
#include "LEDCommand.h"
#include "SmoothPin.h"

enum Modes {
    Off = -1,
    Normal,
    Breathe,
    Rainbow,
    Blink,
    RandomColorSwitch,
};


class LedStrip {
    private:
        Modes currentMode = Modes::Off;
        Modes lastMode = Modes::Off; // used for night mode
        bool nightModeActive = false;
        bool isON = true; // used by blink and breathe
        unsigned long lastBlinkTime = 0; // only used by blink
        uint16_t rainbowHue = 0; // can't do uint8_t because it will overflow
        uint8_t blinkMillis = 250; // used by blink and random color switch
        SimpleColor currentColor;

        SmoothPin redPin;
        SmoothPin greenPin;
        SmoothPin bluePin;
        SmoothPin whitePin;

        std::list<LEDCommand> callList;

        void smoothChangeTo(SimpleColor color, unsigned long delayTime);
        void smoothChangeTo(SimpleColor color);

        void instantChangeTo(SimpleColor color);

        void switchToRainbow();
        void switchToBlink();
        void switchToBreathe();
        void switchToRandomColorSwitch();

        void addToCallList(const LEDCommand& command);
        void changeCallListTo(const LEDCommand& command);
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
         * @brief Instantly change color of led strip
         * @param color color to change to
         * @warning This will not be smooth, use fillColor for smooth color change
        */
        void instantFillColor(SimpleColor color);

        /**
         * @brief Change mode of led strip
         * @param mode mode to change to
        */
        void changeModeTo(Modes mode);

        void setBlinkMillis(uint8_t millis) {
            blinkMillis = millis;
        }

        Modes getCurrentMode() {
            return currentMode;
        };

        SimpleColor getCurrentColor();
        bool isChanging();
        bool isNightMode() const {
            return nightModeActive;
        }
        void getRGB(uint8_t &r, uint8_t &g, uint8_t &b);
};
