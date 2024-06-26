#pragma once
/**
 * @file serialCommandsSystem.h
 * @brief A library for creating commands for serial communication using ArduinoIoTCloud.
 * uses the "messaging" widget from ArduinoIoTCloud. and the "String" object from Arduino.h.
*/
#if !defined(CLOUD_CLI_H) && (defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)) // STD does not exist on Arduino
#define CLOUD_CLI_H
#include <Arduino.h>
#include <Preferences.h>
#include <map>
#include <vector>
#include <queue>

#define delayBetweenPrints 500 // The delay between prints in the print queue in milliseconds

#include "stringTools.h"
class CloudSerialSystem; // Forward declaration

typedef std::function<void(CloudSerialSystem*, std::vector<String>*)> CommandType;
#define command(name) void name(CloudSerialSystem* cloudSerialSystem, std::vector<String>* argv)


class CloudSerialSystem {
    private:
        Preferences preferences;
        String* cloudString{};
        std::map<String, CommandType> commandsList;
        std::queue<String> printBuffer;
        unsigned long lastPrint = 0;
        bool debug = true;
        /**
         * @brief Print the help message for the commands, listing all the commands
         * @note There are currently no descriptions for the commands, may add them in the future
         * @param cloudSerialSystem The CloudSerialSystem object
         * @param argv The arguments passed to the help command
         * @return void
        */
        static command(help);
    public:
    CloudSerialSystem() = default;
    explicit CloudSerialSystem(String* stringToModify);
    void begin(String* stringToModify);
    void addCommand(const String& commandName, CommandType function);
    void checkForCommands(const String& command);
    void checkForCommands() { this->checkForCommands(*this->cloudString); };
    void print(const String& message);
    void debugPrint(const String& message);
    /**
     * @brief Handle the print queue. This should be called in the cloud loop.
    */
    void handlePrintQueue();
    bool getDebug();
    void setDebug(bool debug);
};

extern CloudSerialSystem cloudCLI;
#endif
