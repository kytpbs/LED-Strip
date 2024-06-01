#include "commands/commands.h"

void setupCommands(CloudSerialSystem* cloudSerialSystem, LedStrip* strip) {
    commandHelpers::addAllCommands(cloudSerialSystem);
    stripCommands::setupCommands(strip);
}

