#pragma once

#include "HomeSpan.h"
#include "strip.h"

struct HomeKitStrip : Service::LightBulb {
    LedStrip* ledStrip;

    SpanCharacteristic* on;
    SpanCharacteristic* hue;
    SpanCharacteristic* saturation;
    SpanCharacteristic* brightness;

    HomeKitStrip(LedStrip* strip);

    boolean update();

    void sync();
};
