#include "commands.h"

void addCommand(Command function, String commandName);

// in this define, we add it to the list of commands
static std::map<String, Command> tempCommandsList;
#define command(name) void name(CloudSerialSystem* cloudSerialSystem, std::vector<String>* argv)

// do a dirty constructor getting auto called hack to run addCommand
// this seems to be the only clean way, at least according to my research 
#define createCommand(name) \
command(name); \
struct name##_registrar { \
    name##_registrar() { \
        addCommand(&name, #name); \
    } \
}; \
static name##_registrar _registrar_##name; \
command(name)


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

void setupCommands(CloudSerialSystem* cloudSerialSystem) {
    for (auto const& command : tempCommandsList) {
        cloudSerialSystem->addCommand(command.first, command.second);
    }
    // we clear the list because it will never be used again
    tempCommandsList.clear();
}
