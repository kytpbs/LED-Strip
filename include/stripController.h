#pragma once

#include <Arduino.h>
#include "strip.h"

namespace stripController { 

enum Status {
    UNKNOWN,
    DISCONNECTED,
    CONNECTED,
    SYNCED,
    OTA,
    ERROR = -1,
};

void updateLEDStatus(LedStrip* strip, Status status);

};
