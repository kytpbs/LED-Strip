#include "stripController.h"

void stripController::updateLEDStatus(LedStrip* strip, Status status) {
    switch (status) {
        case CONNECTED: case SYNCED:
            // early return as the strip is already set to the connected color
            return;
            break;
        case UNKNOWN:
            // blink cyan when unknown
            strip->fillColor(SimpleColor(0, 100, 255, 0));
            break;
        case DISCONNECTED:
            // blink red when disconnected
            strip->fillColor(SimpleColor(255, 0, 0, 0));
            break;
        case OTA:
            // blink yellow when in OTA mode
            strip->fillColor(SimpleColor(255, 255, 0, 0));
            break;
        case ERROR:
            // blink orange when in error
            strip->fillColor(SimpleColor(255, 165, 0, 0));
            break;
    }
    strip->changeModeTo(Modes::Blink);
}
