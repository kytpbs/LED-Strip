#include <Arduino.h>
#include "CloudSerial.h" // private library, on lib/CloudSerial
#include "OTA_System.h"
#include "arduino_secrets.h"
#include "Constants.h"
#include "thingProperties.h"
#include "strip.h"
#include "stripController.h"

#include "commands.h"

LedStrip strip(RED_PIN, GREEN_PIN, BLUE_PIN, WHITE_PIN);
stripController::Status ledStatus = stripController::Status::UNKNOWN;
CloudSerialSystem cloudCLI(&cloudSerial);

bool connectedToCloud = false;
bool syncedToCloud = false;

/* START FUNCTION DEFINITIONS */

void setup();
void onCloudSync();
void onCloudConnect();
void onCloudDisconnect();
void cloudSetup();

void loop();
void syncStripToCloud();
void printColorChange();
void printModeChange();
// onVariableChange() functions are defined in thingProperties.h

/* END FUNCTION DEFINITIONS */


void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(115200);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1000);

  strip.instantFillColor(SimpleColor(0, 0, 0, 0));
  // Setup Cloud
  cloudSetup();
  setupCommands(&cloudCLI);
}

void onCloudSync() {
  Serial.println("Synced with IoT Cloud");
  syncedToCloud = true;
  ledStatus = stripController::Status::SYNCED;
}

void onCloudConnect() {
  Serial.println("Connected to IoT Cloud");
  connectedToCloud = true;
  ledStatus = stripController::Status::CONNECTED;
  setupOTA(&cloudCLI);
}

void onCloudDisconnect() {
  Serial.println("Disconnected from IoT Cloud");
  ledStatus = stripController::Status::DISCONNECTED;
  syncedToCloud = false;
}

void cloudSetup() {
  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::CONNECT, onCloudConnect);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::SYNC, onCloudSync);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::DISCONNECT, onCloudDisconnect);
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
  */
  setDebugMessageLevel(0);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  syncStripToCloud();
  ArduinoCloud.update();
  // First updateLEDStatus, as it will change the color of the strip or the mode
  stripController::updateLEDStatus(&strip, ledStatus);
  strip.update();
  if (syncedToCloud) { // only print the queue if we are connected to the cloud, else we will lose the print queue.
    cloudCLI.handlePrintQueue(); // will print the queue if there is something to print, else will do nothing
  }
  // handle OTA updates, only if we are connected to the wifi and the led is not changing due to it taking to much of the loopTime
  if (connectedToCloud && !strip.isChanging()) handleOTA();
}

void syncStripToCloud() {
  // Sync color
  u_int8_t red, green, blue;
  strip.getRGB(red, green, blue);
  float hue, sat, bri;
  ColorUtils::getHSVfromRGB(red, green, blue, hue, sat, bri);
  color.setHue(hue);
  color.setSaturation(sat);
  color.setBrightness(bri);
  // Sync mode
  mode = strip.getCurrentMode();
}

void printColorChange() {
  Serial.println("Color Changed");
  Serial.print("Brightness: ");
  Serial.println(color.getBrightness());
  Serial.print("Saturation: ");
  Serial.println(color.getSaturation());
  Serial.print("Switch: ");
  Serial.println(color.getSwitch());
}

void printModeChange() {
  Serial.print("Mode Changed: ");
  Serial.print((Modes)mode);
  Serial.print(" -> ");
  
  switch ((Modes)mode) {
    case Modes::Normal:
      Serial.println("Normal");
      break;
    case Modes::Breathe:
      Serial.println("Breathe");
      break;
    case Modes::Rainbow:
      Serial.println("Rainbow");
      break;
    case Modes::Blink:
      Serial.println("Blink");
      break;
    case Modes::Off:
      Serial.println("Off");
      break;
  }
}

/*
  Since Color is READ_WRITE variable, onColorChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onColorChange() {
  printColorChange();
  strip.fillColor(SimpleColor(color.getHue(), color.getSaturation(), color.getBrightness()));
}

/*
  Since Mode is READ_WRITE variable, onModeChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onModeChange()  {
  printModeChange();
  // do not call onColorChange when mode is normal, as it will be called by the strip.changeModeTo() function
  strip.changeModeTo(Modes(mode));
}


/*
  Since NightMode is READ_WRITE variable, onNightModeChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onNightModeChange()  {
  Serial.print("Night Mode is ");
  Serial.println(nightMode.isActive() ? "Active" : "Inactive");
  strip.setNightMode(nightMode.isActive());
}

void onCloudSerialChange() {
  Serial.print("New Cloud Serial Command: ");
  Serial.println(cloudSerial);

  cloudCLI.checkForCommands();
}
