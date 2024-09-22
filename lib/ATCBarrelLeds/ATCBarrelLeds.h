#ifndef PROTONPACKV3_ATCBARRELLEDS_H
#define PROTONPACKV3_ATCBARRELLEDS_H

//remove annoying pragma messages
#define FASTLED_INTERNAL

// Load required Dependencies
#include <FastLED.h>

// Max LEDs the Barrel LEDs can have (used for memory allocation)
#define MAX_LED_LENGTH 40

const int BARREL_LED_ROWS[5][8] = {
        {4, 5, 14, 15, 24, 25, 34, 35},
        {3, 6, 13, 16, 23, 26, 33, 36},
        {2, 7, 12, 17, 22, 27, 32, 37},
        {1, 8, 11, 18, 21, 28, 31, 38},
        {0, 9, 10, 19, 20, 29, 30, 39},
};

class ATCBarrelLeds
{
    public:
        // Constructor
        ATCBarrelLeds();

        // Template initializer
        template<uint8_t DATA_PIN> void init(int ledCount, int rowCount) {
            this->_ledCount = ledCount;
            this->_rowCount = rowCount;

            FastLED.addLeds<NEOPIXEL, DATA_PIN>(this->_leds, this->_ledCount);
            FastLED.setBrightness(255);

            clear();
        }

        // Public methods
        bool update(bool force = false);
        void clear();
        void allOn();
        void disable();
        void enable();
        void setColor(CRGB color);
        void setInterval(int interval);
        void setPartyMode(bool isPartyMode);
        void setPartyModeInterval(int interval);
        void setPartyModeGlitter(bool active);

    private:
        int _ledCount;
        int _rowCount;
        int _currentRow = 0;
        int _interval = 500;
        int _partyModeInterval = 20;
        int _currentHue = 0;
        bool _isActive = true;
        bool _isPartyMode = false;
        bool _partyModeGlitter = false;

        unsigned long _currentMillis;
        unsigned long _ledMillis = 0;


        // We default to pure RED for the Barrel LEDs
        CRGB _currentColor = CRGB(255,0,0);
        CRGB _leds[MAX_LED_LENGTH];

        bool _checkTimer(unsigned long &millis, int &interval);
        void _updateCurrentHue();
};


#endif //PROTONPACKV3_ATCBARRELLEDS_H
