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
 * Wand pin 8 (receive, Green Wire) to Pack pin 9 (transmit)
 * Wand pin 9 (transmit, Blue Wire) to Pack pin 8 (receive)
 * IMPORTANT: Pin 10 is NOT available for PWM anymore,
 * but can still be used as a standard Digital IO pin.
 * For more information (including other Arduino devices):
 * https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html
 */
#include <SerialComms.h>
SerialComms pack;

#include "ATCBarrelLeds.h"
#include "ATCNozzleLeds.h"

#include "WandControls.h"
WandControls controls;

/**
 * Neopixels for Barrel and Nozzle
 */
#define BARREL_DATA_PIN 6
#define BARREL_LED_LENGTH 40
#define BARREL_ROW_COUNT 5
ATCBarrelLeds barrel;

#define NOZZLE_DATA_PIN 7
#define NOZZLE_LED_LENGTH 3
ATCNozzleLeds nozzle;

/**
 * Green and Red LEDs on Wand
 */
#define GREEN_LED_PIN 12
#define RED_LED_PIN A1



void setup() {
    if (DEBUG) {
        Serial.begin(115200);
    }

    debugln(F("Initializing ATC Wand V3..."));

    // Initialize Serial Communications with the Pack
    pack.begin(SERIAL_COMMS_BAUD_RATE, SERIAL_COMMS_TERMINATOR);

    controls.init();

    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);

    // Immediately enable Red LED
    // (for now this is used to indicate power, later this could be used for other purposes)
    digitalWrite(RED_LED_PIN, HIGH);

    // Initialize Barrel LEDs (off by default)
    barrel.init<BARREL_DATA_PIN>(BARREL_LED_LENGTH, BARREL_ROW_COUNT);
    barrel.disable();

    // Initialize Nozzle LEDS
    // Initialize Barrel LEDs (off by default)
    nozzle.init<NOZZLE_DATA_PIN>(NOZZLE_LED_LENGTH);
    nozzle.disable();
    debugln(F("ATC Wand V3 Initialized and Ready!"));
}

// send 50 times
const int maxSendAttempts = 50;
int packStateSendsRemaining = 0;
int volumeSendsRemaining = 0;

byte previousPackState = MSG_INACTIVE;
byte currentPackState = MSG_INACTIVE;

// Dummy value to force setting on first boot
byte currentVolume = 0xFF;

bool isActive = false;
void loop() {

    controls.update();

    // Determine current wand state
    currentPackState = controls.readState();

    if (currentPackState != previousPackState)
    {
        // Pack state has changed
        if (isActive && currentPackState == MSG_FIRING)
        {
            nozzle.enable();
            // Increase speed of party mode barrel animation
            barrel.setPartyModeInterval(10);
            barrel.setPartyModeGlitter(true);
        }
        else if (previousPackState == MSG_FIRING)
        {
            // We were firing, stop firing
            nozzle.disable();
            // Reset speed of party mode barrel animation
            barrel.setPartyModeInterval(20);
            barrel.setPartyModeGlitter(false);
        }

        if (currentPackState == MSG_ACTIVE || currentPackState == MSG_MUSIC || currentPackState == MSG_PARTY)
        {
            // Wand mode has changed to Active, Music, or Party Mode.
            barrel.setPartyMode((currentPackState == MSG_PARTY));

            // Start the loop
            barrel.enable();
            digitalWrite(GREEN_LED_PIN, HIGH);
            isActive = true;
        }
        else if (currentPackState == MSG_INACTIVE)
        {
            // Wand state has changed to inactive
            barrel.disable();
            digitalWrite(GREEN_LED_PIN, LOW);
            isActive = false;
        }
    }

    if (currentPackState != previousPackState) {
        // State has changed, reset the packStateSendsRemaining counter to 10
        // so our message is sent to the Pack 10 times
        packStateSendsRemaining = maxSendAttempts;
        previousPackState = currentPackState;
    }

    if (packStateSendsRemaining > 0) {
        // We have incomplete send attempts, send the message and decrement the counter
        pack.send(currentPackState);
        packStateSendsRemaining--;
    }


    // Volume Control
    byte newVolume = controls.readVolume();

    if (newVolume != currentVolume)
    {
        currentVolume = newVolume;
        volumeSendsRemaining = maxSendAttempts;
    }

    if (volumeSendsRemaining > 0) {
        // We have incomplete volume send attempts, send the message and decrement the counter
        pack.send(currentVolume);
        volumeSendsRemaining--;
    }


    if (barrel.update() || nozzle.update()) {
        // We wrote the LEDs
        pack.flushBuffer();
    }
}