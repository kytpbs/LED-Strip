#pragma once
#include <Arduino.h>
Z
typedef std::function<void(void)> Runnable;
class LEDCommand {
    private:
        Runnable taskToRun;
        bool isMultiRun;
        String commandName;
        int runTimes = -1;

    public:
        LEDCommand(Runnable taskToRun, bool isMultiRun, String commandName);
        LEDCommand(Runnable taskToRun, int runTimes, String commandName);
        bool hasEnded();
        void execute();
        String getName();
};
