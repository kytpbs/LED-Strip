#include "CloudSerial.h" // private library, on lib/CloudSerial
#include <WiFi.h>
#include "stringTools.h" // private library, on lib/stringTools


command(ping);
command(echo);
command(reboot);
command(getIP);

void setupCommands(CloudSerialSystem* cloudSerialSystem);
