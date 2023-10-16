#include <Arduino.h>
#include "Common.h"

/**
 * Setup Comms to the Pack
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
SerialComms pack;

/**
 * Button handler (demo only for now)
 */
#define BUTTON_PIN 5
#include <Bounce2.h>
Bounce fireButton = Bounce();

void setup() {
    if (DEBUG) {
        Serial.begin(115200);
    }

    debugln(F("Initializing ATC Wand V3..."));

    // Initialize Serial Communications with the Pack
    pack.begin(SERIAL_COMMS_BAUD_RATE, SERIAL_COMMS_TERMINATOR);

    // Initialize button debouncer
    fireButton.attach(BUTTON_PIN, INPUT_PULLUP);

    // Use a debounce interval of 5 milliseconds
    fireButton.interval(5);

    debugln(F("ATC Wand V3 Initialized and Ready!"));
}

void loop() {

    fireButton.update();
    byte currentState;

    if (!fireButton.read()) {
        // Pressed
        //debugln(F("Pressed!"));
        currentState = STR_MSG_ACTIVE;
    } else {
        // Not pressed
        //debugln(F("Not Pressed!"));
        currentState = STR_MSG_IDLE;
    }
    pack.send(currentState);
}