#include <Arduino.h>
#include "Common.h"

/**
 * Load GhostLab42/Spongeface Display Wrapper Class
 */
#include "ATCDisplays/ATCDisplays.h"
ATCDisplays displays;

/**
 * Load Synchrotron LED class
 */
#include <ATCSynchrotron.h>

// Which PIN is the Synchrotron LED strip attached to?
#define SYNCHROTRON_PIN 6

// How many LEDs in the Synchrotron LED strip/ring?
// NOTE: Your LED ring should be divisible by the SYNCHROTRON_CLUSTER_COUNT
// So a 40 LED ring will contain 8 clusters of 5.
// The only exception is a 60-LED ring, although not perfectly divisible by 8
// we have handled this scenario inside the class (as this is a common LED-ring size)
#define SYNCHROTRON_LED_COUNT 60

// How many LED clusters are there?
// NOTE: In the hero packs there appeared to be 8
#define SYNCHROTRON_CLUSTER_COUNT 8

ATCSynchrotron synchrotron;

void setup() {
    Serial.begin(115200);
    debugln(F("Initializing ATC Pack V3..."));

    // Initialize GhostLab42 Displays
    displays.init();

    // Initialize Synchrotron LEDs
    synchrotron.init<SYNCHROTRON_PIN>(SYNCHROTRON_LED_COUNT, SYNCHROTRON_CLUSTER_COUNT);

    // Which LED should the animation begin at?
    // NOTE: Typically you'll want this to be somewhere in the top-left corner ("North West"),
    // near the CPU/knob box.
    synchrotron.setFirstLedPosition(15);

    synchrotron.setLoopInterval(1600);
    synchrotron.setClusterInterval(100);
    synchrotron.setFadeIncrement(2);

    // Trigger an immediate update of the Synchrotron
    synchrotron.update(true);

    debugln(F("ATC Pack V3 Initialized and Ready!"));
}

void loop() {
    displays.update();
    synchrotron.update();
}
