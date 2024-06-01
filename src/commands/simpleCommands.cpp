#include "CloudSerial.h" // private library, on lib/CloudSerial
#include <WiFi.h>
#include "stringTools.h" // private library, on lib/stringTools

#include "commands/commandHelper.h"


// in this define, we add it to the list of commands
static std::map<String, Command> tempCommandsList;

createCommand(ping) {
    cloudSerialSystem->print("pong");
}

createCommand(echo) {
    if (argv->size() == 0) {
        cloudSerialSystem->print("No arguments provided!");
        return;
    }
    cloudSerialSystem->print(joinString(argv, " "));
}

createCommand(reboot) {
    cloudSerialSystem->print("Rebooting...");
    ESP.restart();
}

createCommand(getIP) {
    cloudSerialSystem->print(WiFi.localIP().toString());
}

createCommand(setDebugMode) {
    if (argv->size() == 0) {
        cloudSerialSystem->print("No arguments provided! Current debug mode: " + String(cloudSerialSystem->getDebug() ? "true" : "false"));
        return;
    }
    String argument = argv->at(0);
    if (argument == "true" || argument == "on") {
        cloudSerialSystem->setDebug(true);
        cloudSerialSystem->print("Debug mode set to true");
        return;
    }
    if (argument == "false" || argument == "off") {
        cloudSerialSystem->setDebug(false);
        cloudSerialSystem->print("Debug mode set to false");
        return;
    }
    cloudSerialSystem->print("Invalid argument! Not changing debug mode. Current debug mode: " + String(cloudSerialSystem->getDebug() ? "true" : "false"));
}

void addCommand(Command function, String commandName) {
    tempCommandsList[commandName] = function;
}


namespace simpleCommands {
    void setupCommands(CloudSerialSystem* cloudSerialSystem) {
        for (auto const& command : tempCommandsList) {
            cloudSerialSystem->addCommand(command.first, command.second);
        }
        // we clear the list because it will never be used again
        tempCommandsList.clear();
    }
}