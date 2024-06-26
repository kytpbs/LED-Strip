#include "stringTools.h"

#include <utility>
#ifndef STRINGTOOLS_H // STD does not exist on Arduino
// we don't need to implement this for Arduino, because std is not available on Arduino.
#else // STD does not exist on Arduino


void splitString(String string, std::vector<String>* argv) {
    int spaceIndex = string.indexOf(" ");
    
    if (spaceIndex == -1) {
        return; // If there are no spaces, there are no arguments so we can return
    }

    String args = string.substring(spaceIndex + 1); // Removing the command name from the string, +1 to remove the space as well
    
    while ((spaceIndex = args.indexOf(" ")) != -1) {
        Serial.println("Space index: " + String(spaceIndex));
        if (spaceIndex >= args.length()) {
            Serial.println("ERROR: Space index out of bounds! Args: " + args + " Space index: " + String(spaceIndex) + " Length: " + String(args.length()));
            return;
        }
        argv->push_back(args.substring(0, spaceIndex));
        if (spaceIndex + 1 >= args.length()) {
            Serial.println("ERROR: Substring out of bounds! Args: " + args + " Space index: " + String(spaceIndex) + " Length: " + String(args.length()));
            return;
        }
        args = args.substring(spaceIndex + 1); // +1 to remove the space as well.
    }
    Serial.println("Args: " + args);
    argv->push_back(args);
}

std::vector<String> splitStringtoVec(String string) {
    std::vector<String> argv = std::vector<String>();
    splitString(std::move(string), &argv);
    return argv;
}

String joinString(std::vector<String>* argv, String separator) {
    // return String("Not implemented yet!");
    String string = "";
    for (int i = 0; i < argv->size(); i++) {
        string = string + argv->at(i);
        if (i != argv->size() - 1) {
            string += separator;
        }
    }
    return string;
}

String joinString(std::vector<String>* argv, char separator) {
    return joinString(argv, String(separator));
}

String joinString(std::vector<String>* argv) {
    return joinString(argv, " ");
}

size_t getDistanceBetweenStrings(String a, String b) {
    size_t minSize = std::min(a.length(), b.length());
    size_t maxSize = std::max(a.length(), b.length());

    size_t distance = 0;
    for (int i = 0; i < minSize; i++) {
        if (a[i] != b[i]) {
            distance++;
        }
    }
    distance += maxSize - minSize;

    return distance;
}


std::string toStdString(const String& string) {
    return std::string(string.c_str());
}

std::tuple<String, int> fuzzyFind(std::vector<String> searchFrom, String toMatch) {
    const size_t n = searchFrom.size();
    size_t distances[n];

    size_t minDistanceIndex = 0;

    for (int i = 0; i < n; i++) {
        auto distance = getDistanceBetweenStrings(toMatch, searchFrom[i]);
        distances[i] = distance;
        if (distance < distances[minDistanceIndex]) {
            minDistanceIndex = i;
        }
    }

    String resultString = searchFrom[minDistanceIndex];
    std::tuple<String, int> result = std::make_tuple(resultString, distances[minDistanceIndex]);
    return result;
}
#endif
