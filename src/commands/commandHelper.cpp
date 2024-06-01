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
    }
}
