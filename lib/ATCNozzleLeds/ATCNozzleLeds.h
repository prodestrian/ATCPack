#ifndef PROTONPACKV3_ATCNOZZLELEDS_H
#define PROTONPACKV3_ATCNOZZLELEDS_H

//remove annoying pragma messages
#define FASTLED_INTERNAL

// Load required Dependencies
#include <FastLED.h>

// Max LEDs the Barrel LEDs can have (used for memory allocation)
#define MAX_NOZZLE_LED_LENGTH 7

#define FIRING_COLOR_COUNT 15


const CRGB firingColors[FIRING_COLOR_COUNT][3] = {
        {CRGB::White, CRGB::White, CRGB::White},
        {CRGB::Black, CRGB::Black, CRGB::Black},
        {CRGB::White, CRGB::White, CRGB::White},
        {CRGB::Black, CRGB::Black, CRGB::Black},
        {CRGB::White, CRGB::White, CRGB::White},
        {CRGB::Black, CRGB::Black, CRGB::Black},
        {CRGB::Blue, CRGB::Red, CRGB::Magenta},
        {CRGB::Black, CRGB::Black, CRGB::Black},
        {CRGB::Red, CRGB::Red, CRGB::Red},
        {CRGB::Magenta, CRGB::Magenta, CRGB::Black},
        {CRGB::Blue, CRGB::Red, CRGB::Magenta},
        {CRGB::Red, CRGB::Black, CRGB::Blue},
        {CRGB::Blue, CRGB::Red, CRGB::Magenta},
        {CRGB::Red, CRGB::Black, CRGB::Magenta},
        {CRGB::Magenta, CRGB::Green, CRGB::Red},
};

class ATCNozzleLeds
{
    public:
        // Constructor
        ATCNozzleLeds();

        // Template initializer
        template<uint8_t DATA_PIN> void init(int ledCount) {
            this->_ledCount = ledCount;

            FastLED.addLeds<NEOPIXEL, DATA_PIN>(this->_leds, this->_ledCount);
            FastLED.setBrightness(255);

            clear();
        }

        // Public methods
        bool update(bool force = false);
        void clear();
        void disable();
        void enable();
        void setColor(CRGB color);
        void setInterval(int interval);

    private:
        int _ledCount;
        int _currentState = 0;
        int _interval = 80;
        bool _isActive = true;

        unsigned long _currentMillis;
        unsigned long _ledMillis = 0;

        // We default to pure WHITE for the Nozzle LEDs
        CRGB _currentColor = CRGB(255,255,255);
        CRGB _leds[MAX_NOZZLE_LED_LENGTH];

        bool _checkTimer(unsigned long &millis, int &interval);
};


#endif //PROTONPACKV3_ATCNOZZLELEDS_H
