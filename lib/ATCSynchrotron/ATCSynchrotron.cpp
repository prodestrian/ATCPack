#include "ATCSynchrotron.h"

ATCSynchrotron::ATCSynchrotron()
{
    //
};

bool ATCSynchrotron::update(bool force)
{
    _currentMillis = millis();

    if (_isPartyMode) {
        _updateCurrentHue();
        fill_rainbow_circular(_leds, _ledCount, _currentHue, 7);
        // Add random glitter
        _leds[ random16(_ledCount) ] += CRGB::White;
    } else {
        if (_checkTimer(_loopMillis, _loopInterval) || force)
        {
            // Loop timer has elapsed so we need to kick off a new cycling loop now
            if (_isActive)
            {
                _isCycling = true;
            }
        }

        if (_checkTimer(_clusterMillis, _clusterInterval) || force)
        {
            // Cluster millis has elapsed so we need to loop through the clusters
            _updateClusterBrightness();
        }

        _updateLedBrightness();
    }

    if (_checkTimer(_ledMillis, _ledInterval) || force) {
        // Write to the LED strip
        FastLED.show();

        // Return true as we wrote LEDs in this cycles
        return true;
    }

    // No LEDs were written in this cycle
    return false;
};

void ATCSynchrotron::clear()
{
    FastLED.clear();
    FastLED.show();
};

void ATCSynchrotron::disable()
{
    _isActive = false;
};

void ATCSynchrotron::enable()
{
    _isActive = true;
    // Trigger an immediate update
    update(true);
};

void ATCSynchrotron::setLoopInterval(int loopInterval)
{
    _loopInterval = loopInterval;
};

void ATCSynchrotron::setClusterInterval(int clusterInterval)
{
    _clusterInterval = clusterInterval;
};

void ATCSynchrotron::setFadeIncrement(int fadeIncrement)
{
    _fadeIncrement = fadeIncrement;
};

int ATCSynchrotron::_getClusterLength(int clusterIndex)
{
    return _ledCount / _clusterCount;
}
int ATCSynchrotron::_getFirstIndexInCluster(int clusterIndex)
{
    if (_ledCount == 60 && _clusterCount == 8) {
        // Special case for 60 LEDs divided into 8 Clusters
        // Returns 0,8,15,23,30,38,45,53
        return LED_CLUSTERS_60_8[clusterIndex][0];
    }

    return clusterIndex * _getClusterLength(clusterIndex);

}

int ATCSynchrotron::_getLastIndexInCluster(int clusterIndex)
{
    if (_ledCount == 60 && _clusterCount == 8) {
        // Special case for 60 LEDs divided into 8 Clusters
        // Returns 7,14,22,29,37,44,52,59
        return LED_CLUSTERS_60_8[clusterIndex][1];
    }

    return _getFirstIndexInCluster(clusterIndex) + (_getClusterLength(clusterIndex) - 1);
}

bool ATCSynchrotron::_isLedInCluster(int ledIndex, int clusterIndex)
{
    // LED index is greater than or equal to the first index in the Cluster,
    // and also less than or equal to the last index in the Cluster
    return ledIndex >= _getFirstIndexInCluster(clusterIndex)
    && ledIndex <= _getLastIndexInCluster(clusterIndex);
};

void ATCSynchrotron::setColor(CRGB color)
{
    _currentColor = color;
};

void ATCSynchrotron::setPartyMode(bool isPartyMode)
{
    _isPartyMode = isPartyMode;
};

bool ATCSynchrotron::_checkTimer(unsigned long &millis, int &interval)
{
    if (_currentMillis - millis >= (unsigned long) interval)
    {
        // Timer has elapsed
        millis = _currentMillis;
        return true;
    }

    return false;
}

void ATCSynchrotron::setFirstLedPosition(int firstLedPosition)
{
    if (firstLedPosition < 0 || firstLedPosition >= _ledCount)
    {
        // The first LED can only be from 0 to (_ledCount-1).
        // So for a 60-LED strip, the first LED position can only be 0-59
        return;
    }

    _firstLedPosition = firstLedPosition;
}

int ATCSynchrotron::_getOffsetLedPosition(int currentPosition)
{
    int newPosition = currentPosition + _firstLedPosition;
    if (newPosition >= _ledCount)
    {
        newPosition = newPosition - _ledCount;
    }

    return newPosition;
}

void ATCSynchrotron::_updateClusterBrightness()
{
    if (!_isCycling)
    {
        return;
    }

    if (_currentCluster == _clusterCount)
    {
        _currentCluster = 0;
        _isCycling = false;
    }
    else
    {
        _clusterBrightness[_currentCluster] = 1;
        _currentCluster++;
    }
}

void ATCSynchrotron::_updateCurrentHue()
{
    if (_currentHue ==0)
    {
        _currentHue = 255;
    } else {
        // We go backwards because the rainbow effect looks reversed otherwise
        _currentHue--;
    }
}

void ATCSynchrotron::_updateLedBrightness()
{
    for (int clusterIndex = 0; clusterIndex < _clusterCount; clusterIndex++)
    {
        if (_clusterBrightness[clusterIndex] > 0)
        {
            // If the brightness is > 0 then we are fading up
            _clusterBrightness[clusterIndex] += _fadeIncrement;
        }
        if (_clusterBrightness[clusterIndex] >= 510)
        {
            // If the brightness is >= 510 then we have finished fading out and can go back to 0
            _clusterBrightness[clusterIndex] = 0;
        }

        // Calculate the actual brightness from 0-255
        int brightness = _clusterBrightness[clusterIndex];
        if (brightness > 255)
        {
            // We are fading back out
            brightness = 255 - (brightness - 255);
        }

        for (int i = 0; i < _ledCount; i++)
        {
            // Loop over all LEDs

            if (!_isLedInCluster(i, clusterIndex))
            {
                // This LED is outside the current Cluster, skip over it
                continue;
            }

            // Determine the LED position taking into account the configured offset
            int ledPosition = _getOffsetLedPosition(i);

            // Set to current Color before setting brightness
            _leds[ledPosition] = _currentColor;

            // Reduce color to X% (eg 192/256ths) of its previous value
            // eventually fading to full black
            _leds[ledPosition].nscale8( brightness);
        }
    }
}