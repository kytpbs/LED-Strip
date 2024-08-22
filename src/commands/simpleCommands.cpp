#include "commands/simpleCommands.h"

command(ping) {
    cloudSerialSystem->print("pong");
}

command(echo) {
    if (argv->empty()) {
        cloudSerialSystem->print("No arguments provided!");
        return;
    }
    cloudSerialSystem->print(joinString(argv, " "));
}

command(reboot) {
    cloudSerialSystem->print("Rebooting...");
    ESP.restart();
}

command(getIP) {
    cloudSerialSystem->print(WiFi.localIP().toString());
}

command(setDebugMode) {
    if (argv->empty()) {
        cloudSerialSystem->print("No arguments provided! Current debug mode: " + String(cloudSerialSystem->getDebug() ? "true" : "false"));
        return;
    }
    const String argument = argv->at(0);
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

command(getTemp) {
    cloudSerialSystem->print(String(temperatureRead()) + "°C");
}

namespace simpleCommands {
    void setupCommands(CloudSerialSystem* cloudSerialSystem) {
        cloudSerialSystem->addCommand("ping", ping);
        cloudSerialSystem->addCommand("echo", echo);
        cloudSerialSystem->addCommand("reboot", reboot);
        cloudSerialSystem->addCommand("getIP", getIP);
        cloudSerialSystem->addCommand("setDebugMode", setDebugMode);
        cloudSerialSystem->addCommand("getTemp", getTemp);
    }
}
