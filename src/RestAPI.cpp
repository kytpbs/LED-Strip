#include <ArduinoJson.h>

#include "OTA_System.h"
#include "strip.h"

LedStrip* stripPointer;

JsonDocument fetchColorAsJson();

void setupAPI(LedStrip& ledStrip) {
    stripPointer = &ledStrip;
    // Other Stuff
    server.on("/api/color", HTTP_GET, [] () {
        char colorJsonBuffer[256]; // If this is over 256 we are cooked...
        auto colorJson = fetchColorAsJson();
        serializeJson(colorJson, colorJsonBuffer);
        server.send(200, "application/json", colorJsonBuffer);
    });

    server.on("/api/color", HTTP_POST, [] () {
        if (server.hasArg("plain")) {
            auto inputJsonString = server.arg("plain");
            JsonDocument colorJson;
            DeserializationError error = deserializeJson(colorJson, inputJsonString);
            auto hue = colorJson["hue"];
            auto saturation = colorJson["saturation"];
            auto brightness = colorJson["brightness"];
            bool is_valid = hue.is<int>() && saturation.is<int>() && brightness.is<int>();
            if (error || !is_valid) {
                server.send(400, "application/json", "{\"error\": \"Invalid JSON\"}");
            } else {
                stripPointer->fillColor(SimpleColor(hue, saturation, brightness));
                server.send(200, "application/json", "{\"success\": \"Color Updated\"}");
            }
        } else {
            server.send(400, "application/json", "{\"error\": \"No JSON Provided\"}");
        }
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
