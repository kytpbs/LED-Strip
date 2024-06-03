#pragma once

#include <WiFi.h>
#include "stringTools.h" // private library, on lib/stringTools

#include "commands/commandHelper.h"

namespace simpleCommands {
    void setupCommands(CloudSerialSystem* cloudSerialSystem);
}
