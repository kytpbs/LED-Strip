#include "commands/stripCommands.h"

LedStrip* m_strip;

command(fillColor) {
    if (argv->size() < 3) {
        cloudSerialSystem->print("Usage: fillColor <red> <green> <blue> ?<white>");
        return;
    }

    uint8_t red = argv->at(0).toInt();
    uint8_t green = argv->at(1).toInt();
    uint8_t blue = argv->at(2).toInt();
    uint8_t white = 0;

    // if size is 4, then white is provided
    if (argv->size() > 3) {
        white = argv->at(3).toInt();
    }

    SimpleColor color = SimpleColor(red, green, blue, white);
    m_strip->fillColor(color);

    cloudSerialSystem->print("Filled with color: " + color.toHexRGB());
}

command(setMode) {
    if (argv->empty()) {
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
