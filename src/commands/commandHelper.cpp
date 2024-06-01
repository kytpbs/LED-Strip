#include "commands/commandHelper.h"

std::map<String, Command> tempCommandsList;
namespace commandHelpers {
    void addCommand(Command function, String commandName) {
        tempCommandsList[commandName] = function;
    };

    void addAllCommands(CloudSerialSystem* cloudSerialSystem) {
        for (auto const& command : tempCommandsList) {
            cloudSerialSystem->addCommand(command.first, command.second);
        }
        // Clear the tempCommandsList because we will never use it again
        // There is no easy way to delete the map, so we just clear it
        tempCommandsList.clear();
    }
}
