#include "commands.h"

// in this define, we add it to the list of commands
#define command(name) void name(CloudSerialSystem* cloudSerialSystem, std::vector<String>* argv)

command(ping) {
    cloudSerialSystem->print("pong");
}

command(echo) {
    if (argv->size() == 0) {
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


void setupCommands(CloudSerialSystem* cloudSerialSystem) {
    cloudSerialSystem->addCommand("ping", ping);
    cloudSerialSystem->addCommand("echo", echo);
    cloudSerialSystem->addCommand("reboot", reboot);
    cloudSerialSystem->addCommand("getIP", getIP);
}
