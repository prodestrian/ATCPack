#include "ATCBarrelLeds.h"

ATCBarrelLeds::ATCBarrelLeds()
{
    // Constructor
};

bool ATCBarrelLeds::update(bool force)
{
    if (!_isActive) {
        // Barrel LEDs are not active
        return false;
    }

    _currentMillis = millis();

    if (_isPartyMode && _checkTimer(_ledMillis, _partyModeInterval))
    {
        _updateCurrentHue();
        fill_rainbow(_leds, _ledCount, _currentHue, 7);
        if (_partyModeGlitter)
        {
            // Add random glitter
            _leds[random16(_ledCount)] += CRGB::White;
        }

        FastLED.show();
        return true;
    }

    if (_currentRow >= _rowCount) {
        // We have ended the animation
        return false;
    }

    if (_checkTimer(_ledMillis, _interval) || force) {

        for (int row = 0; row < _rowCount; ++row)
        {
            for (size_t i = 0; i < 8; i++)
            {
                _leds[BARREL_LED_ROWS[row][i]] = (row <= _currentRow) ? _currentColor : CRGB::Black;
            }
        }

        _currentRow++;

        // Write to the LED strip
        FastLED.show();

        // Return true as we wrote LEDs in this cycles
        return true;
    }

    // No LEDs were written in this cycle
    return false;
};

void ATCBarrelLeds::clear()
{
    _currentRow = 0;
    for (int i = 0; i < _ledCount; ++i)
    {
        _leds[i] = CRGB::Black;
    }

    FastLED.show();
};

void ATCBarrelLeds::allOn()
{
    _currentRow = _rowCount;
    for (int i = 0; i < _ledCount; ++i)
    {
        _leds[i] = _currentColor;
    }

    FastLED.show();
}

void ATCBarrelLeds::disable()
{
    _isActive = false;
    clear();
};

void ATCBarrelLeds::enable()
{
    _isActive = true;
    // Trigger an immediate update
    update(false);
};

void ATCBarrelLeds::setInterval(int interval)
{
    _interval = interval;
};

void ATCBarrelLeds::setPartyMode(bool isPartyMode)
{
    if (_isPartyMode && !isPartyMode)
    {
        // We were in Party Mode but it has now been switched off
        allOn();
    }

    _isPartyMode = isPartyMode;
};

void ATCBarrelLeds::setPartyModeInterval(int interval)
{
    _partyModeInterval = interval;
};


void ATCBarrelLeds::setPartyModeGlitter(bool active)
{
    _partyModeGlitter = active;
};

void ATCBarrelLeds::_updateCurrentHue()
{
    if (_currentHue <= 0)
    {
        _currentHue = 255;
    } else {
        // We go backwards because the rainbow effect looks reversed otherwise
        _currentHue-=3;
    }
};

bool ATCBarrelLeds::_checkTimer(unsigned long &millis, int &interval)
{
    if (_currentMillis - millis >= (unsigned long) interval)
    {
        // Timer has elapsed
        millis = _currentMillis;
        return true;
    }

    return false;
};
