#include "CloudSerial.h"
#ifdef SERIALCOMMANDSSYSTEM_H // only include this file if serialCommandsSystem.h is included

bool firstRun = true;

CloudSerialSystem::CloudSerialSystem(String* cloudSerialObject) {
    this->cloudString = cloudSerialObject;
    this->addCommand("help", help);
}

void CloudSerialSystem::help(CloudSerialSystem* cloudSerialSystem, std::vector<String>* argv) {
    cloudSerialSystem->print("Commands:");
    for (auto const& command : cloudSerialSystem->commandsList) {
        cloudSerialSystem->print(command.first); //! Warning: this might print a lot of text, may want to change this in the future
        // cloudSerialSystem->print(command.first + " - " + command.second.description); // TODO: add description support to commands
    }
}

void CloudSerialSystem::addCommand(String commandName, Command function) {
    this->commandsList[commandName] = function;
}

void CloudSerialSystem::print(String message) {
    this->printBuffer.push(message);
}

void CloudSerialSystem::checkForCommands(String command) {
    int spaceIndex = command.indexOf(" ");
    bool hasArgs = spaceIndex != -1;

    if (spaceIndex == -1) {
        spaceIndex = command.length();
    }

    String commandName = command.substring(0, spaceIndex);
    if (this->commandsList.find(commandName) != this->commandsList.end()) {
        std::vector<String> argv;
        if (hasArgs) { // Split the string into arguments, if it has any
            splitString(command.substring(spaceIndex), &argv);
        }
        Serial.println("Running command + " + commandName + " with " + String(argv.size()) + " arguments");
        this->commandsList[commandName](this, &argv);
    }
    else {
        std::vector<String> commandNames;
        for (auto const& command : this->commandsList) {
            commandNames.push_back(command.first);
        }
        Serial.println(commandName + " named Command not found");
        auto result = fuzzyFind(commandNames, commandName);
        String name = std::get<0>(result);
        int distance = std::get<1>(result);
        if (distance <= commandName.length() / 4) {
            this->print("You misspelled '"+ name +"', running that instead.");
            checkForCommands(name + command.substring(spaceIndex));
        } else {
            this->print(
                "Command not found, did you mean: '" + 
                name + "'? (" + String(distance) + " distance)");
        }
    }
}

void CloudSerialSystem::debugPrint(String message) {
    if (this->getDebug()) {
        this->print("DEBUG - " + message); // only print to cloudSerial if debug is enabled
    }
    Serial.println("DEBUG - " + message); // Always print to serial
}

void CloudSerialSystem::setDebug(bool debug) {
    this->debug = debug;
    this->preferences.begin("cloudSerial", false);
    this->preferences.putBool("debug", debug);
    this->preferences.end();
}

bool CloudSerialSystem::getDebug() {
    if (firstRun) {
        this->preferences.begin("cloudSerial", true);
        this->debug = this->preferences.getBool("debug", true);
        this->preferences.end();
    }
    return this->debug;
} 

void CloudSerialSystem::handlePrintQueue() {
    if (this->printBuffer.size() > 0 && millis() - this->lastPrint > delayBetweenPrints) {
        String message = this->printBuffer.front();
        Serial.println("Printing message: \"" + message + "\" to cloudSerial");
        this->printBuffer.pop();
        this->cloudString->operator= (message);

        this->lastPrint = millis();
    }
}

#endif
