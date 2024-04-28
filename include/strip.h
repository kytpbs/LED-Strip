#pragma once
#include <algorithm>
#include <ArduinoIoTCloud.h>

// Log data to serial if VERBOSE is defined
//* Info logs will print data once to the serial, while VERBOSE logs will print data repeatedly.
// Please use INFO for important logs, and VERBOSE for debugging.
#ifdef VERBOSE
#define LOG(x) Serial.println(x)
#define LOG_INFO(x) Serial.println(x)
#else
#define LOG(x)
#endif
#ifdef INFO
#define LOG_INFO(x) Serial.println(x)
#else
#define LOG_INFO(x)
#warning "INFO is not defined, this will cause some logs to not be printed"
#endif

enum Modes {
  Normal,
  Breathe,
  Rainbow,
  Off
};

namespace arrayFunctions {
  /**
   * @brief turns (0 - maxValue) into (maxValue - 0)
   * @warning if the value is bigger than maxValue, it will be negative.
   * If it is negative, it will be bigger than maxValue
  */
  void reverseValues(int* array, int arraySize, int maxValue);

  void reverseValues(int* array, int arraySize);

  void getDifference(int* array1, int* array2, int* result, int arraySize);

  int* getDifference(int* array1, int* array2, int arraySize);
}

class LedStrip {
    public:
        LedStrip(uint8_t redPin, uint8_t greenPin, uint8_t bluePin, uint8_t whitePin);
        uint32_t Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t white);
        uint32_t Color(uint8_t red, uint8_t green, uint8_t blue);
        uint32_t Color(CloudColoredLight* color);
        /**
         * @brief Update led strip, must be called in loop repeatedly.
         * @note Rainbow and Breathe effects will not work if this is not called in loop.
         * Normal mode will work without calling this in loop.
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
         * @note This will work if update() is not called in loop,
        */
        void changeLedColor(uint32_t color);
        /**
         * @brief Change color of led strip
         * @param color color to change to
         * @note This will work if update() is not called in loop,
         * and the function itself shouldn't be called in loop.
        */
        void changeLedColor(CloudColoredLight* color);
        /**
         * @brief Change mode of led strip
         * @param mode mode to change to
         * @note This will not work if update() or this function is not called in loop
        */
        void changeMode(Modes mode);
        /**
         * @brief Do rainbow effect, must be called in loop repeatedly
         * @note This will not work if update() or this function is not called in loop
        */
        void doRainbow();
        /**
         * @brief Do rainbow effect, must be called in loop repeatedly
        */
        void doRainbow(CloudColoredLight* color);
        /**
         * @brief Do rainbow effect, must be called in loop repeatedly
         * @param delayTime delay time in milliseconds
        */
        void doRainbow(CloudColoredLight* color, unsigned long delayTime);
        /**
         * @brief Do breathe effect, must be called in loop repeatedly
         * @note This will not work if update() or this function is not called in loop
        */
        void breathe();
        /**
         * @brief Do breathe effect, must be called in loop repeatedly
         * @param delayTime delay time in milliseconds
        */
        void breathe(unsigned long delayTime);

    private:
        Modes currentMode;
        Modes lastMode; // used for night mode
        bool nightModeActive;
        uint rainbowHue;
        uint32_t color;

        uint8_t redPin;
        uint8_t greenPin;
        uint8_t bluePin;
        uint8_t whitePin;

        void SetupLedPins();
        void smoothChangeTo(uint32_t color);
        void smoothChangeTo(uint32_t color, unsigned long delayTime);
        void smoothChangeTo(uint8_t red, uint8_t green, uint8_t blue, uint8_t white);
        void smoothChangeTo(uint8_t red, uint8_t green, uint8_t blue, uint8_t white, unsigned long delayTime);
        void smoothChangeTo(int* newColors, int* oldColors, unsigned long delayTime);
        int* getAllDifferences(uint32_t color);
        int* getAllDifferences(uint8_t red, uint8_t green, uint8_t blue, uint8_t white);
        /**
         * @brief Get the All Differences object
         * @param red red color value
         * @param green green color value
         * @param blue blue color value
         * @param white white color value
         * @param differences array of differences (array size must be 4 or greater)
        */
        void getAllDifferences(uint8_t red, uint8_t green, uint8_t blue, uint8_t white, int* differences);
};
