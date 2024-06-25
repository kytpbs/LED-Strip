#include "commands/commands.h"

void setupCommands(CloudSerialSystem* cloudSerialSystem, LedStrip* strip) {
    cloudCLI.debugPrint("Setting up commands, please wait...");
    stripCommands::setupCommands(cloudSerialSystem, strip);
    simpleCommands::setupCommands(cloudSerialSystem);
}
