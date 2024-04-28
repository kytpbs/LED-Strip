#include "CloudSerial.h" // private library, on lib/CloudSerial
#include "stringTools.h"
#define command(name) void name(CloudSerialSystem* cloudSerialSystem, std::vector<String>* argv)

command(ping);
command(echo);
command(reboot);
command(getIP);

void setupCommands(CloudSerialSystem* cloudSerialSystem);
