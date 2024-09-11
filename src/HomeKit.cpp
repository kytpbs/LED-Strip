#include "HomeSpan.h"
#include "HomeKit.h"


void HomeKit::setupHomeKit() {
    homeSpan.begin(Category::Lighting, "LED Strip");

    new SpanAccessory();

    {
        new Service::AccessoryInformation();
        new Characteristic::Identify();
        new Characteristic::Model("RGBW Strip");
        new Characteristic::On();
        new Characteristic::Hue(180); // default value of CYAN
        new Characteristic::Saturation(100); // default value of CYAN
        new Characteristic::Brightness(100); // default value of ON
    }
}

void HomeKit::update() {
    homeSpan.poll();
}

