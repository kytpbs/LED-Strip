#include "OTA_System.h"
#ifdef OTA_SYSTEM_H
#ifdef ARDUINO_ARCH_ESP8266
    ESP8266WebServer server(80);
#else // We are on an ESP32
    WebServer server(80);
#endif

void setupOTA() {

    cloudCLI.debugPrint("Setting up Server...");
    cloudCLI.debugPrint("Connected to WiFi: \"" + WiFi.SSID() + "\"");
    cloudCLI.print("CURRENT IP: " + WiFi.localIP().toString());

    server.on("/", []() {
        server.sendHeader("Location", "/update");
        server.send(302);
    });

    // ArduinoOTA.setPassword(PASSWORD);
    ElegantOTA.setAuth(USERNAME, PASSWORD);

    ElegantOTA.begin(&server);    // Start ElegantOTA

    server.begin();
    cloudCLI.debugPrint("OTA/HTTP server started");

    ArduinoOTA.onStart(onOTAStart);
    ArduinoOTA.onEnd([]() {onOTAEnd(true);});
    ArduinoOTA.onError([](ota_error_t) {onOTAEnd(false);});
    ArduinoOTA.begin();

    ElegantOTA.onStart(onOTAStart);
    ElegantOTA.onEnd(onOTAEnd);
}

void handleOTA() {
    server.handleClient();
    ElegantOTA.loop();
    ArduinoOTA.handle();
}

void onOTAStart() {
    cloudCLI.print("OTA UPLOAD STARTED");
}

void onOTAEnd(bool success) {
    if (success) {
        cloudCLI.print("OTA UPLOAD COMPLETE! REBOOTING...");
    }
    else {
        cloudCLI.print("OTA Failed PLEASE TRY AGAIN");
    }
}

#endif
