#include "HomeKitStrip.h"
#include "CloudSerial.h"


HomeKitStrip::HomeKitStrip(LedStrip* strip) : Service::LightBulb() {
    on = new Characteristic::On();
    hue = new Characteristic::Hue();
    saturation = new Characteristic::Saturation();
    brightness = new Characteristic::Brightness();

    this->ledStrip = strip;
}

boolean HomeKitStrip::update() {
    cloudCLI.debugPrint("update received from HomeKit");
    if (!on->getNewVal()) {
        ledStrip->changeModeTo(Modes::Off);
        return true;
    }

    if (ledStrip->getCurrentMode() == Modes::Off) {
        ledStrip->changeModeTo(Modes::Normal);
    }

    ledStrip->fillColor(SimpleColor(hue->getNewVal(), saturation->getNewVal(), brightness->getNewVal()));
    return true;
}

void HomeKitStrip::sync() {
    on->setVal(ledStrip->getCurrentMode() != Modes::Off);
    auto color = ledStrip->getCurrentColor();
    hue->setVal(color.getHue());
    saturation->setVal(color.getSaturation());
    brightness->setVal(color.getBrightness());
}
