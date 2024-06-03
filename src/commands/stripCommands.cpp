#include "commands/stripCommands.h"

LedStrip* m_strip;

command(fillColor) {
    if (argv->size() < 4) {
        cloudSerialSystem->print("Usage: fillColor <red> <green> <blue> <white>");
        return;
    }

    int red = argv->at(0).toInt();
    int green = argv->at(1).toInt();
    int blue = argv->at(2).toInt();
    int white = argv->at(3).toInt();

    m_strip->fillColor(SimpleColor(red, green, blue, white));
}

command(setMode) {
    if (argv->size() == 0) {
        cloudSerialSystem->print("Usage: setMode <mode>");
        return;
    }

    String mode = argv->at(0);
    Modes modeEnum;
    if (mode == "normal") {
        modeEnum = Modes::Normal;
        cloudSerialSystem->print("Setting mode to normal");
    }
    else if (mode == "breathe") {
        modeEnum = Modes::Breathe;
        cloudSerialSystem->print("Setting mode to breathe");
    }
    else if (mode == "rainbow") {
        modeEnum = Modes::Rainbow;
        cloudSerialSystem->print("Setting mode to rainbow");
    }
    else if (mode == "blink") {
        modeEnum = Modes::Blink;
        cloudSerialSystem->print("Setting mode to blink");
    }
    else if (mode == "off") {
        modeEnum = Modes::Off;
        cloudSerialSystem->print("Setting mode to off");
    } 
    else {
        cloudSerialSystem->print("Invalid mode: " + mode);
        return;
    }
    m_strip->changeModeTo(modeEnum);
}

namespace stripCommands {
    void setupCommands(CloudSerialSystem* cloudSerialSystem, LedStrip* ledStrip) {
        m_strip = ledStrip;
        cloudSerialSystem->addCommand("fillColor", fillColor);
        cloudSerialSystem->addCommand("setMode", setMode);
    }
}
