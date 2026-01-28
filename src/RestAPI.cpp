#include <ArduinoJson.h>
#include <SPIFFS.h>

#include <CloudSerial.h> // private library, on lib/CloudSerial

#include "OTA_System.h"
#include "strip.h"


LedStrip* stripPointer;

/****      FUNCTION DEFINITIONS      ****/
JsonDocument fetchColorAsJson();
/****  END OF FUNCTION DEFINITIONS   ****/


void setupAPI(LedStrip& ledStrip) {
    stripPointer = &ledStrip;
    if (!SPIFFS.begin(true)) {
        cloudCLI.debugPrint("Failed to mount file system, will not be able to serve files");
    }
    // I setup the main app here, but it can be done in any other place

    server.on("/app", HTTP_GET, [] () {
        File file = SPIFFS.open("/index.html", "r");
        if (!file || file.isDirectory() || file.size() == 0) {
            server.send(500, "text/html", "Failed to open file");
            cloudCLI.debugPrint("Failed to open index.html file");
            return;
        }
        server.send(200, "text/html", file.readString());
    });

    server.on("/api/color", HTTP_GET, [] () {
        char colorJsonBuffer[256]; // If this is over 256 we are cooked...
        auto colorJson = fetchColorAsJson();
        serializeJson(colorJson, colorJsonBuffer);
        server.send(200, "application/json", colorJsonBuffer);
    });

    server.on("/api/color", HTTP_POST, [] () {
        if (!server.hasArg("plain")) {
            server.send(400, "application/json", "{\"error\": \"No JSON Provided\"}");
            return;
        }
        auto inputJsonString = server.arg("plain");
        JsonDocument colorJson;
        DeserializationError error = deserializeJson(colorJson, inputJsonString);

        const auto& hue = colorJson["hue"];
        const auto& saturation = colorJson["saturation"];
        const auto& brightness = colorJson["brightness"];
        bool is_valid = hue.is<int>() && saturation.is<int>() && brightness.is<int>();

        if (error || !is_valid) {
            server.send(400, "application/json", "{\"error\": \"Invalid JSON\"}");
            return;
        }

        stripPointer->fillColor(SimpleColor(hue, saturation, brightness));
        server.send(200, "application/json", "{\"success\": \"Color Updated\"}");
    });

    server.on("/api/night-mode", HTTP_GET, [] () {
        String night_mode_status = stripPointer->isNightMode() ? "true" : "false";
        server.send(200, "application/json", "{\"nightMode\": " + night_mode_status + "}");
    });

    server.on("/api/night-mode", HTTP_POST, [] () {
        if (!server.hasArg("plain")) {
            server.send(400, "application/json", "{\"error\": \"No JSON Provided\"}");
            return;
        }
        auto inputJsonString = server.arg("plain");
        JsonDocument nightModeJson;
        DeserializationError error = deserializeJson(nightModeJson, inputJsonString);

        const auto& nightMode = nightModeJson["nightMode"];
        bool is_valid = nightMode.is<bool>();

        if (error || !is_valid) {
            server.send(400, "application/json", "{\"error\": \"Invalid JSON\"}");
            return;
        }

        stripPointer->setNightMode(nightMode);
        server.send(200, "application/json", "{\"success\": \"Night Mode Updated\"}");
    });

    server.on("/api/ping", HTTP_GET, [] () {
        server.send(200, "application/json", "{\"message\": \"Pong\"}");
    });

    server.on("/api/test", HTTP_GET, [] () {
        File file = SPIFFS.open("/test.json", "r");
        if (!file) {
            server.send(500, "application/json", "{\"error\": \"Failed to open file\"}");
            cloudCLI.debugPrint("Failed to open test json file");
            return;
        }
        server.send(200, "application/json", file.readString());
    });
}

JsonDocument fetchColorAsJson() { 
    auto color = stripPointer->getCurrentColor();
    JsonDocument colorJson;

    colorJson["hue"] = color.getHue();
    colorJson["saturation"] = color.getSaturation();
    colorJson["brightness"] = color.getBrightness();

    return colorJson;
}
