#include "commands/commands.h"

void setupCommands(CloudSerialSystem* cloudSerialSystem, LedStrip* strip) {
    Serial.println("Setting up commands, please wait...");
    stripCommands::setupCommands(cloudSerialSystem, strip);
    simpleCommands::setupCommands(cloudSerialSystem);
}
