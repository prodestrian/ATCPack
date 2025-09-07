#include <Arduino.h>
#include "Common.h"
/**
 * Master Pin Map
 * D0 - Reserved Tx
 * D1 - Reserved Rx
 * D2 - Rotary Encoder Button 1 (Orange)
 * D3 - Rotary Encoder 1A (Blue)
 * D4 - Rotary Encoder 1B (Green)
 * D5 - Rotary Encoder Button 2 (Orange)
 * D6 - Rotary Encoder 2A (Yellow)
 * D7 - Rotary Encoder 2B (Brown)
 * D8 - Software Serial Rx (from Wand 9 Tx, AltSoftSerial pin can not be changed, Blue Wire)
 * D9 - Software Serial Tx (to Wand 8 Rx, AltSoftSerial pin can not be changed, Green Wire)
 * D10 - Audio Rx (from DFPlayer)
 * D11 - Audio Tx (from DFPlayer)
 * D12 - Audio Busy (from DFPlayer)
 * D13 - Synchrotron Neopixel strip
 * A0 -
 * A1 -
 * A2 -
 * A3 -
 * A4 - i2c SDA
 * A5 - i2c SCL
 * A6 -
 * A7 -
 */
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
#define SYNCHROTRON_PIN 13

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

// SoftwareSerial used for Audio Player
#include <SoftwareSerial.h>
#include "AudioPlayer.h"
AudioPlayer audioPlayer;
SoftwareSerial audioSerial(10, 11); // RX, TX

// Rotary Encoders
/**
 * TODO: The encoders will be used to set the number on the top display.
 * They unfortunately take forever to scroll through, so setting values from 000-999 is going
 * to be too slow. Instead, we should set one digit at a time (from 0-9).
 * This allows us to also include a null/blank value in each digit if needed.
 * Hold down both buttons then release to enter "Set" mode.
 * The top display will blank out, and a "cursor" will appear on the first digit (flashing hyphen?).
 * We could use GhostLab42Reboot.write("-") to write non-numeric characters here.
 * Use the first rotary encoder to select the character to display (scroll up/down).
 * Use the second rotary encoder to select which digit to edit (0 through 6).
 * Press either button to exit "Set" mode.
 */
//#include <ClickEncoder.h>
////#include <TimerOne.h>
//#define ROTARY_A_BUTTON 2 // D2- Rotary Encoder Button 1 (Orange)
//#define ROTARY_A_PIN1 3 // D3 - Rotary Encoder 1A (Blue)
//#define ROTARY_A_PIN2 4 // D4 - Rotary Encoder 1B (Green)
//#define ROTARY_B_BUTTON 5// D5 - Rotary Encoder Button 2 (Orange)
//#define ROTARY_B_PIN1 6// D6 - Rotary Encoder 2A (Yellow)
//#define ROTARY_B_PIN2 7// D7 - Rotary Encoder 2B (Brown)
//
//ClickEncoder *encoderA;
//ClickEncoder *encoderB;
////void timerIsr() {
////    encoderA->service();
////    encoderB->service();
////}
//int16_t oldEncPosA, encPosA;
//int16_t oldEncPosB, encPosB;

void setup() {
    //if (DEBUG) {
        Serial.begin(115200);
    //}

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
        synchrotron.setLoopInterval(1800);
        synchrotron.setClusterInterval(120);
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

    // Initialize Audio Player
    audioSerial.begin(9600);
    audioPlayer.begin(audioSerial, false);
    audioPlayer.stop();

    // Initialize Rotary Encoders
    // NOTE: We have pins 1/2 reversed because of how they were wired up
    // Steps currently set to 2 but this needs to be tested
//    encoderA = new ClickEncoder(ROTARY_A_PIN2, ROTARY_A_PIN1, ROTARY_A_BUTTON, 2);
//    encoderB = new ClickEncoder(ROTARY_B_PIN2, ROTARY_B_PIN1, ROTARY_B_BUTTON, 2);
//    Timer1.initialize(1000);
//    Timer1.attachInterrupt(timerIsr);
//    encoderA->setAccelerationEnabled(true);
//    encoderB->setAccelerationEnabled(true);
//    oldEncPosA = -1;
//    oldEncPosB = -1;

    debugln(F("ATC Pack V3 Initialized and Ready!"));
}

STATES currentState;
STATES previousState;

bool isFiring = false;
bool wasFiring = false;

bool isPlayingMusic = false;

void loop() {

    displays.update();

    wand.update();
//
//    static uint32_t lastService = 0;
//    if (micros() - lastService >= 1000) {
//        lastService = micros();
//        encoderA->service();
//        //encoderB->service();
//    }
//
//    encPosA += encoderA->getValue();
//    //encPosB += encoderB->getValue();
//    if (encPosA != oldEncPosA) {
//        oldEncPosA = encPosA;
//        Serial.print("Encoder A Value: ");
//        Serial.println(encPosA);
//    }
////    if (encPosB != oldEncPosB) {
////        oldEncPosB = encPosB;
////        Serial.print("Encoder B Value: ");
////        Serial.println(encPosB);
////    }

//    if (encoderA->getButton() == ClickEncoder::Clicked) {
//        Serial.println(F("Button A Pressed!"));
//    }
//    if (encoderB->getButton() == ClickEncoder::Clicked) {
//        Serial.println(F("Button B Pressed!"));
//    }

    if (wand.changed()) {
        // The Wand has sent a new message

        wasFiring = isFiring;
        isFiring = false;

        debug(F("Wand sent state: "));
        if (wand.isMessage(MSG_INACTIVE)) {
            previousState = currentState;
            currentState = STATE_INACTIVE;
            debugln(F("Inactive"));
        } else if(wand.isMessage(MSG_ACTIVE)) {
            previousState = currentState;
            currentState = STATE_ACTIVE;
            debugln(F("Active"));
        } else if (wand.isMessage(MSG_MUSIC)) {
            previousState = currentState;
            currentState = STATE_MUSIC;
            debugln(F("Music"));
        } else if (wand.isMessage(MSG_PARTY)) {
            previousState = currentState;
            currentState = STATE_PARTY;
            debugln(F("Party"));
        } else if (wand.isMessage(MSG_VOL_00)) {
            audioPlayer.setVolume(0);
            debugln(F("Volume 0"));
        } else if (wand.isMessage(MSG_VOL_01)) {
            audioPlayer.setVolume(3);
            debugln(F("Volume 3"));
        } else if (wand.isMessage(MSG_VOL_02)) {
            audioPlayer.setVolume(6);
            debugln(F("Volume 6"));
        } else if (wand.isMessage(MSG_VOL_03)) {
            audioPlayer.setVolume(9);
            debugln(F("Volume 9"));
        } else if (wand.isMessage(MSG_VOL_04)) {
            audioPlayer.setVolume(12);
            debugln(F("Volume 12"));
        } else if (wand.isMessage(MSG_VOL_05)) {
            audioPlayer.setVolume(15);
            debugln(F("Volume 15"));
        } else if (wand.isMessage(MSG_VOL_06)) {
            audioPlayer.setVolume(18);
            debugln(F("Volume 18"));
        } else if (wand.isMessage(MSG_VOL_07)) {
            audioPlayer.setVolume(21);
            debugln(F("Volume 21"));
        } else if (wand.isMessage(MSG_VOL_08)) {
            audioPlayer.setVolume(24);
            debugln(F("Volume 24"));
        } else if (wand.isMessage(MSG_VOL_09)) {
            audioPlayer.setVolume(27);
            debugln(F("Volume 27"));
        } else if (wand.isMessage(MSG_VOL_10)) {
            audioPlayer.setVolume(30);
            debugln(F("Volume 30"));
        } else if (wand.isMessage(MSG_FIRING)) {
            // Fire
            isFiring = true;
            debugln(F("Firing"));
        } else {
            debugln(F("Unknown"));
        }

        // Handle changes in pack states
        if (currentState != previousState)
        {
            debug(F("State Changed from "));
            debug(previousState);
            debug(F(" to "));
            debugln(currentState);

            // Pack state changed
            if (currentState == STATE_INACTIVE)
            {
                // Pack is off
                // Play shutdown sound
                audioPlayer.playTrack(SOUND_PACK_SHUTDOWN);
            } else if (previousState == STATE_INACTIVE) {
                // Pack was inactive and is now active
                // Play startup sound
                audioPlayer.playTrack(SOUND_PACK_STARTUP);
            } else if (previousState == STATE_PARTY || previousState == STATE_MUSIC) {
                // We were probably playing music, we should stop
                // This includes changing from Party to Music mode or back again
                audioPlayer.stop();
                isPlayingMusic = false;
            }
        }

        // Set Synchrotron Party Mode state
        synchrotron.setPartyMode(currentState == STATE_PARTY);

        // Handle firing sounds
        if (isFiring && !wasFiring) {
            // We started firing

            if (currentState == STATE_ACTIVE) {
                // We are in Active mode
                // Play firing sound
                audioPlayer.playTrack(SOUND_PACK_FIRING);
                debugln(F("Start firing"));
            }
            else if (currentState == STATE_MUSIC)
            {
                // We are in Music Mode
                if (!isPlayingMusic)
                {
                    // Start playback of folder 02
                    audioPlayer.playMp3Folder(2);
                    isPlayingMusic = true;
                } else {
                    // Play next track in folder 02
                    audioPlayer.nextTrack(2);
                }
            } else if (currentState == STATE_PARTY) {
                // We are in Party Mode
                if (!isPlayingMusic)
                {
                    // Play song in folder 3
                    audioPlayer.playTrackInFolder(3, 1);
                    isPlayingMusic = true;
                }
                // NOTE: If fire button is pressed while in party mode we don't
                //       do anything, this allows the firing animation to play without constantly
                //       resetting the track
            }
        } else if (!isFiring && wasFiring) {
            // We stopped firing
            if (currentState == STATE_ACTIVE) {
                // Play stop firing sound
                debugln(F("Stop firing"));
                audioPlayer.playTrack(SOUND_PACK_FIRE_STOP);
            }
            // NOTE: We don't do anything with "stop firing" in the other modes.
        }

        // Handle firing synchrotron speed
        if (isFiring) {
            synchrotron.setLoopInterval(600);
            synchrotron.setClusterInterval(40);
            synchrotron.setFadeIncrement(6);
        } else if (wasFiring) {
            // Back to original speeds
            synchrotron.setLoopInterval(1800);
            synchrotron.setClusterInterval(120);
            synchrotron.setFadeIncrement(2);
        }
    }

    if (synchrotron.update()) {
        // Clear the Serial buffer of any possible garbage values
        wand.flushBuffer();
    }

    previousState = currentState;
}
