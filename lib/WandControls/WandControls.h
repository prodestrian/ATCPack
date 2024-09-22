#ifndef PROTONPACKV3_WANDCONTROLS_H
#define PROTONPACKV3_WANDCONTROLS_H

#include "Arduino.h"
#include <Bounce2.h>
#include <Smoothed.h>
#include "SerialMessages.h"
/**
 * 3 toggle switches on wand
 */
#define SWITCH1_PIN 2
#define SWITCH2_PIN 3
#define SWITCH3_PIN 4

/**
 * Trigger push button on wand
 */
#define TRIGGER_PIN 10

/**
 * Analog Potentiometer
 */
#define POT_PIN A0

class WandControls
{
public:
    WandControls();

    void init();
    void update();
    byte readState();
    byte readVolume();

private:
    Bounce _switch1;
    Bounce _switch2;
    Bounce _switch3;
    Bounce _trigger;
    Smoothed <float> _volumeKnob;
};
#endif //PROTONPACKV3_WANDCONTROLS_H
