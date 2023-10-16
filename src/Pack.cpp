#include <Arduino.h>
#include "Common.h"

/**
 * Load GhostLab42/Spongeface Display Wrapper Class
 */
#include <ATCDisplays.h>
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

/**
 * Setup Comms from the Wand
 *
 * !! IMPORTANT NOTE !!
 * As we are using the AltSoftSerial library under the hood,
 * this means the Rx/Tx pins are locked at the hardware level and can not be changed.
 *
 * For an Arduino Nano you should connect:
 * Pack pin 8 (receive) to Wand pin 9 (transmit)
 * Pack pin 9 (transmit) to Wand pin 8 (receive)
 * IMPORTANT: Pin 10 is NOT available for PWM anymore,
 * but can still be used as a standard Digital IO pin.
 * For more information (including other Arduino devices):
 * https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html
 */
#include <SerialComms.h>
SerialComms wand;

void setup() {
    if (DEBUG) {
        Serial.begin(115200);
    }

    debugln(F("Initializing ATC Pack V3..."));

    // Initialize Serial Communications with the Wand
    wand.begin(SERIAL_COMMS_BAUD_RATE, SERIAL_COMMS_TERMINATOR);

    // Initialize GhostLab42 Displays
    displays.init();

    // Initialize Synchrotron LEDs
    synchrotron.init<SYNCHROTRON_PIN>(SYNCHROTRON_LED_COUNT, SYNCHROTRON_CLUSTER_COUNT);

    // Which LED should the animation begin at?
    // NOTE: Typically you'll want this to be somewhere in the top-left corner ("North West"),
    // near the CPU/knob box.
    synchrotron.setFirstLedPosition(15);

    // 60 LED ring
    if (SYNCHROTRON_LED_COUNT == 60)
    {
        synchrotron.setLoopInterval(1600);
        synchrotron.setClusterInterval(100);
        synchrotron.setFadeIncrement(2);
    } else
    {
        // 40 LED ring
        synchrotron.setLoopInterval(1600);
        synchrotron.setClusterInterval(100);
        synchrotron.setFadeIncrement(4);
    }

    // Trigger an immediate update of the Synchrotron
    synchrotron.update(true);

    debugln(F("ATC Pack V3 Initialized and Ready!"));
}

int changeCounter = 0;
void loop() {

    displays.update();

    wand.update();

    if (wand.changed()) {
        /**
         * TODO: We should refactor this to use numeric codes instead of strings
         * These should be defined within SerialMessages as Enums.
         * We could then set the max code and reject any value which is not within this range (eg 0-10).
         * We should NOT set wand.changed() to true if an invalid value was received.
         */
        changeCounter++;
        debug("Changed: ");
        debugln(changeCounter);
        // The Wand has sent a new message
        if (wand.isMessage(STR_MSG_ACTIVE)) {
            synchrotron.setColor(CRGB(255,0,0));
//            displays.enable();
//            synchrotron.enable();

            debugln("Active");
        } else if (wand.isMessage(STR_MSG_IDLE)) {
            synchrotron.setColor(CRGB(0,255,0));
//            displays.disable();
//            synchrotron.disable();
            debugln("Idle");
        } else {
            debugln(wand.message());
        }
    }

    if (synchrotron.update()) {
        // Clear the Serial buffer of any possible garbage values
        wand.flushBuffer();
        // Skip the rest of the loop if we wrote to the LEDs
        return;
    }
}
