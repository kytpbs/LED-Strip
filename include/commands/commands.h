#include <Arduino.h>
#include <CloudSerial.h>
#include "strip.h"

#include "commands/commandHelper.h"
#include "commands/stripCommands.h"

void setupCommands(CloudSerialSystem* cloudSerialSystem, LedStrip* strip);
