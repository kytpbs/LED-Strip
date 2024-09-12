#include "HomeSpan.h"
#include "HomeKit.h"
#include "HomeKitStrip.h"
#include "CloudSerial.h"

HomeKitStrip* homeKitStrip;

void HomeKit::setupHomeKit(LedStrip* strip) {
    cloudCLI.debugPrint("Setting up HomeKit, please wait...");
    homeSpan.begin(Category::Lighting, "LED Strip");

    new SpanAccessory();
    new Service::AccessoryInformation();
    new Characteristic::Identify();
    new Characteristic::Model();

    homeKitStrip = new HomeKitStrip(strip);
    cloudCLI.debugPrint("HomeKit setup complete!");
}

void HomeKit::update() {
    homeSpan.poll();
    homeKitStrip->sync();
}
