#include "ATCNozzleLeds.h"

ATCNozzleLeds::ATCNozzleLeds()
{
    // Constructor
};

bool ATCNozzleLeds::update(bool force)
{
    if (!_isActive) {
        // Nozzle LEDs are not active
        return false;
    }

    _currentMillis = millis();

    if (_checkTimer(_ledMillis, _interval) || force) {

        for (int i = 0; i < _ledCount; ++i)
        {
            _leds[i] = firingColors[_currentState][i];
        }

        _currentState++;
        if (_currentState >= FIRING_COLOR_COUNT) {
            _currentState = 0;
        }

        // Write to the LED strip
        FastLED.show();

        // Return true as we wrote LEDs in this cycles
        return true;
    }

    // No LEDs were written in this cycle
    return false;
};

void ATCNozzleLeds::clear()
{
    _currentState = 0;
    for (int i = 0; i < _ledCount; ++i)
    {
        _leds[i] = CRGB::Black;
    }
    FastLED.show();
};

void ATCNozzleLeds::disable()
{
    _isActive = false;
    clear();
};

void ATCNozzleLeds::enable()
{
    _isActive = true;
    // Trigger an immediate update
    update(false);
};

void ATCNozzleLeds::setInterval(int interval)
{
    _interval = interval;
};

bool ATCNozzleLeds::_checkTimer(unsigned long &millis, int &interval)
{
    if (_currentMillis - millis >= (unsigned long) interval)
    {
        // Timer has elapsed
        millis = _currentMillis;
        return true;
    }

    return false;
};
