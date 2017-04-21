/*********************************************************************************************************************
 ** 2017-04-21, RDU: Sample to use a photocell
 *********************************************************************************************************************/
#include <Homie.h>


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Globals
  // Software specifications
    #define FW_NAME    "D1Mini-Photocell"
    #define FW_VERSION "0.17.4.21"


  // Photocell
    #define PHOTOCELL_PIN D2

    // Sensor consts/vars
      HomieNode photocellNode("lightLevel", "lightLevel");

    // Measure loop
        const int MEASURE_INTERVAL = 5; // How often to poll sensor (seconds)
        unsigned long lastMeasureSent = 0;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Homie Setup Handler
void setupHandler() {
  // Nodes part
  photocellNode.setProperty("unit").send("on-off");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Homie loop Handler
void loopHandler() {
  if (millis() - lastMeasureSent >= MEASURE_INTERVAL * 1000UL || lastMeasureSent == 0) {
    int lightLevel = digitalRead(PHOTOCELL_PIN) ;
    if (isnan(lightLevel)) {
      Homie.getLogger() << F("Failed to read from sensor!");
    } else {
      Homie.getLogger() << F("Light level: ") << lightLevel << endl;
      Homie.getLogger() << F("Light is   : ") << (lightLevel==0?"on":"off") << endl;
      photocellNode.setProperty("lightLevel").send(String(lightLevel));
    }
    lastMeasureSent = millis();
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Arduino Setup Handler
void setup() {
  Serial.begin(115200); // Required to enable serial output

  Homie_setFirmware(FW_NAME, FW_VERSION);
  Homie.setSetupFunction(setupHandler).setLoopFunction(loopHandler);

  // DS18B20 sensor
    photocellNode.advertise("unit");
    photocellNode.advertise("lightLevel");

  Homie.setup();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Arduino Loop Handler
void loop() {
  Homie.loop();
}
