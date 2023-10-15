#include "ATCDisplays.h"

ATCDisplays::ATCDisplays()
{
    // Do nothing
}

void ATCDisplays::init()
{
    _device.begin();

    // Turn down the brightness of the main displays (0-100)
    _device.setDisplayBrightness(_topDisplay, 60);
    _device.setDisplayBrightness(_midDisplay, 80);
    _device.setDisplayBrightness(_lowerDisplay, 60);
}

void ATCDisplays::update()
{
    if (_paused || !_checkTimer()) {
        return;
    }

    String countStr;

    // Count down
    _device.write(_topDisplay, String(120999 - _currentCount));

    // Count up with leading 0s really fast
    countStr = "000" + String(16 * _currentCount);
    _device.write(_midDisplay, countStr.substring(countStr.length() - 4));

    // Show a number that is about 2087 but moves around randomly by a few counts
    // Slow the update down, so that it changes more slowly
    if ((_currentCount % 30) == 0)
    {
        _count = 2087 + random(-2, 3);
    }
    if (_count != _lastCount)
    {
        _device.write(_lowerDisplay, String(_count));
        _lastCount = _count;
    }

    _currentCount++;
    if (_currentCount == 1000)
    {
        _currentCount = 0;
    }
}

void ATCDisplays::pause()
{
    _paused = true;
}

void ATCDisplays::resume()
{
    _paused = false;
}

void ATCDisplays::disable()
{
    // Pause all updates
    pause();

    // Write an empty string to blank all displays
    // (6 spaces is long enough to blank out the longest display)
    _writeAll("      ");
}

void ATCDisplays::enable()
{
    _lastCount = 0;
    resume();
}

bool ATCDisplays::_checkTimer()
{
    unsigned long currentMillis = millis();
    if (currentMillis - _previousMillis >= (unsigned long)_speed)
    {
        _previousMillis = currentMillis;
        return true;
    }

    return false;
}

void ATCDisplays::_writeAll(String value)
{
    _device.write(_topDisplay, value);
    _device.write(_midDisplay, value);
    _device.write(_lowerDisplay, value);
}
