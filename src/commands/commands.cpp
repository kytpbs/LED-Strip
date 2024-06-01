#include "commands/commands.h"

void setupCommands(CloudSerialSystem* cloudSerialSystem, LedStrip* strip) {
    stripCommands::setupCommands(strip);
    commandHelpers::addAllCommands(cloudSerialSystem); //! should be always at the end
}
