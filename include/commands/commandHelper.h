#pragma once
#ifndef COMMANDHELPER_H
#define COMMANDHELPER_H
#include <Arduino.h>
#include <vector>
#include <CloudSerial.h>

#define command(name) void name(CloudSerialSystem* cloudSerialSystem, std::vector<String>* argv)

#endif
