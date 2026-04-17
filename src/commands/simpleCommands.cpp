#include "commands/simpleCommands.h"

#include <WiFi.h>

#include "Constants.h"


command(ping) {
    cloudSerialSystem->print("pong");
}

command(echo) {
    if (argv->empty()) {
        cloudSerialSystem->print("No arguments provided!");
        return;
    }
    cloudSerialSystem->print(joinString(argv, " "));
}

command(reboot) {
    cloudSerialSystem->print("Rebooting...");
    ESP.restart();
}

command(getIP) {
    cloudSerialSystem->print(WiFi.localIP().toString());
}

command(setDebugMode) {
    if (argv->empty()) {
        cloudSerialSystem->print("No arguments provided! Current debug mode: " + String(cloudSerialSystem->getDebug() ? "true" : "false"));
        return;
    }
    const String argument = argv->at(0);
    if (argument == "true" || argument == "on") {
        cloudSerialSystem->setDebug(true);
        cloudSerialSystem->print("Debug mode set to true");
        return;
    }
    if (argument == "false" || argument == "off") {
        cloudSerialSystem->setDebug(false);
        cloudSerialSystem->print("Debug mode set to false");
        return;
    }
    cloudSerialSystem->print("Invalid argument! Not changing debug mode. Current debug mode: " + String(cloudSerialSystem->getDebug() ? "true" : "false"));
}

command(getTemp) {
    cloudSerialSystem->print(String(temperatureRead()) + "°C");
}

union MacAddress {
    uint8_t bytes[6];
    struct {
        uint8_t b0, b1, b2, b3, b4, b5;
    };

    String toString() {
        char buf[18];
        snprintf(buf, sizeof(buf), "%02X:%02X:%02X:%02X:%02X:%02X", b0, b1, b2, b3, b4, b5);
        return String(buf);
    }

    static bool parse(const String &str, MacAddress &mac) {
        return (sscanf(str.c_str(), "%x:%x:%x:%x:%x:%x", &mac.b0, &mac.b1, &mac.b2,
                       &mac.b3, &mac.b4, &mac.b5) == 6);
    }
};

// bc-e9-2f-7c-de-f0
static const MacAddress DEFAULT_WOL_MAC = {DEFAULT_WOL_MAC_ADDRESS};

static void sendMagicPacket(const MacAddress& mac) {
    uint8_t packet[102];
    memset(packet, 0xFF, 6);
    for (int i = 1; i <= 16; ++i) {
        memcpy(&packet[i * 6], mac.bytes, 6);
    }

    WiFiUDP udp;
    udp.begin(9);
    udp.beginPacket(IPAddress(255, 255, 255, 255), 9);
    udp.write(packet, sizeof(packet));
    udp.endPacket();
    udp.stop();
}

command(wol) {
    MacAddress mac = DEFAULT_WOL_MAC;

    if (!argv->empty() && !MacAddress::parse(argv->at(0), mac)) {
        cloudSerialSystem->print(
            "Invalid MAC address! Use format AA:BB:CC:DD:EE:FF");
        return;
    }

    sendMagicPacket(mac);
    cloudSerialSystem->print("WOL packet sent to " + mac.toString());
}

namespace simpleCommands {
    void setupCommands(CloudSerialSystem* cloudSerialSystem) {
        cloudSerialSystem->addCommand("ping", ping);
        cloudSerialSystem->addCommand("echo", echo);
        cloudSerialSystem->addCommand("reboot", reboot);
        cloudSerialSystem->addCommand("getIP", getIP);
        cloudSerialSystem->addCommand("setDebugMode", setDebugMode);
        cloudSerialSystem->addCommand("getTemp", getTemp);
        cloudSerialSystem->addCommand("wol", wol);
    }
}
