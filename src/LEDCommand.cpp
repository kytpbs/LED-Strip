#include "ledCommand.h"

LEDCommand::LEDCommand(Runnable taskToRun, bool isMultiRun, String commandName) {
    this->taskToRun = taskToRun;
    this->isMultiRun = isMultiRun;
    this->commandName = commandName;
}

LEDCommand::LEDCommand(Runnable toRun, int runTimes, String name) {
    this->taskToRun = toRun;
    this->commandName = name;
    this->runTimes = runTimes;

    this->isMultiRun = true;
}

bool LEDCommand::hasEnded() {
    if (!isMultiRun) return true;

    // has run the specified amount
    if (runTimes == 0) return true;

    // reduce the amount of runTimes
    if (runTimes > 0) runTimes--;

    // Continue
    return false;
}

void LEDCommand::execute() {
    this->taskToRun();
}

String LEDCommand::getName() {
    return this->commandName;
}
