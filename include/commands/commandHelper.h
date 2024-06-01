#pragma once
#include <Arduino.h>
#include <CloudSerial.h>

namespace commandHelpers {
    std::map<String, Command> tempCommandsList;
    
    void addCommand(Command function, String commandName) {
        commandHelpers::tempCommandsList[commandName] = function;
    };

    void addAllCommands(CloudSerialSystem* cloudSerialSystem) {
        for (auto const& command : commandHelpers::tempCommandsList) {
            cloudSerialSystem->addCommand(command.first, command.second);
        }
    }
}
#define command(name) void name(CloudSerialSystem* cloudSerialSystem, std::vector<String>* argv)

// do a dirty constructor getting auto called hack to run addCommand
// this seems to be the only clean way, at least according to my research 
#define createCommand(name) \
command(name); \
struct name##_registrar { \
    name##_registrar() { \
        commandHelpers::addCommand(&name, #name); \
    } \
}; \
static name##_registrar _registrar_##name; \
command(name)
