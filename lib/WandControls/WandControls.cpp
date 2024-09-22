#include "WandControls.h"

WandControls::WandControls()
{
    //
};

void WandControls::init()
{
    _switch1 = Bounce();
    _switch2 = Bounce();
    _switch3 = Bounce();
    _trigger = Bounce();

    // Initialize button debouncers
    _switch1.attach(SWITCH1_PIN, INPUT_PULLUP);
    _switch2.attach(SWITCH2_PIN, INPUT_PULLUP);
    _switch3.attach(SWITCH3_PIN, INPUT_PULLUP);
    _trigger.attach(TRIGGER_PIN, INPUT_PULLUP);

    // Use a debounce interval of 5 milliseconds
    _switch1.interval(5);
    _switch2.interval(5);
    _switch3.interval(5);
    _trigger.interval(5);

    _volumeKnob.begin(SMOOTHED_AVERAGE, 10);
}

void WandControls::update()
{
    _switch1.update();
    _switch2.update();
    _switch3.update();
    _trigger.update();
};

byte WandControls::readState()
{
    byte state = MSG_INACTIVE;

    // Determine current wand state
    if (!_switch1.read())
    {
        // Switch 1 is enabled, wand is active
        state = MSG_ACTIVE;

        if (!_switch2.read())
        {
            // Switch 2 is enabled, wand is in music mode
            state = MSG_MUSIC;

            if (!_switch3.read())
            {
                // Switch 3 is enabled, wand is in party mode
                state = MSG_PARTY;
            }
        }

        if (!_trigger.read())
        {
            // Trigger was pressed
            state = MSG_FIRING;
        }
    }

    return state;
};

byte WandControls::readVolume()
{
    // Read the volume knob
    int rawVolume = analogRead(A0);
    // Smooth the values
    _volumeKnob.add(rawVolume);
    // Convert to a 0-10 range
    int volume = map(_volumeKnob.get(), 0, 980, 0, 10);

    byte newVolume;

    switch (volume)
    {
        case 0:
            newVolume = MSG_VOL_00;
            break;
        case 1:
            newVolume = MSG_VOL_01;
            break;
        case 2:
            newVolume = MSG_VOL_02;
            break;
        case 3:
            newVolume = MSG_VOL_03;
            break;
        case 4:
            newVolume = MSG_VOL_04;
            break;
        case 5:
            newVolume = MSG_VOL_05;
            break;
        case 6:
            newVolume = MSG_VOL_06;
            break;
        case 7:
            newVolume = MSG_VOL_07;
            break;
        case 8:
            newVolume = MSG_VOL_08;
            break;
        case 9:
            newVolume = MSG_VOL_09;
            break;
        case 10:
            newVolume = MSG_VOL_10;
            break;
    }

    return newVolume;
}