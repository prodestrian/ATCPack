#ifndef ATCSYNCHROTRON_H
#define ATCSYNCHROTRON_H

//remove annoying pragma messages
#define FASTLED_INTERNAL

// Load required Dependencies
#include <FastLED.h>

// Max LEDs the Synchrotron can have (used for memory allocation)
// NOTE: This doesn't need to be changed unless you plan to run an LED ring LARGER than 60-LEDS.
// This will work fine for a 40-LED ring (or any other size equal or less than 60)
#define MAX_LED_LENGTH 60

// Max number of grouped LED clusters we support (used for memory allocation)
// NOTE: This doesn't need to be changed as most of the time we use <12, typically only 8 clusters
#define MAX_CLUSTER_COUNT 12

/**
 * Because 60-LED rings are a commonly available size, but it isn't perfectly divisible by 8,
 * we have a predefined array of start/end LED clusters if the user happens to have that setup.
 */
const int LED_CLUSTERS_60_8[8][2] = {
     {0, 7},   // 8
     {8, 14},  // 7
     {15, 22}, // 8
     {23, 29}, // 7
     {30, 37}, // 8
     {38, 44}, // 7
     {45, 52}, // 8
     {53, 59}, // 7
};

class ATCSynchrotron
{
public:
        // Constructor
        ATCSynchrotron();
        // Template initializer
        template<uint8_t DATA_PIN> void init(int ledCount, int clusterCount) {
            this->_ledCount = ledCount;
            this->_clusterCount = clusterCount;

            FastLED.addLeds<NEOPIXEL, DATA_PIN>(this->_leds, this->_ledCount);
            clear();
        }

        // Public methods
        bool update(bool force = false);
        void clear();
        void disable();
        void enable();
        void setLoopInterval(int loopInterval);
        void setClusterInterval(int clusterInterval);
        void setFadeIncrement(int fadeIncrement);
        void setColor(CRGB color);
        void setFirstLedPosition(int firstLedPosition);
    private:
        int _ledCount;
        int _clusterCount;
        int _currentCluster = 0;
        int _loopInterval = 1500;
        int _clusterInterval = 100;
        int _ledInterval = 20;
        int _fadeIncrement = 2;
        int _firstLedPosition = 0;
        bool _isActive = true;
        bool _isCycling = false;

        unsigned long _currentMillis;
        unsigned long _loopMillis = 0;
        unsigned long _clusterMillis = 0;
        unsigned long _ledMillis = 0;
        uint16_t _clusterBrightness[MAX_CLUSTER_COUNT] = {0};

        // We default to pure RED for the Synchrotron
        CRGB _currentColor = CRGB(255,0,0);
        CRGB _leds[MAX_LED_LENGTH];

        bool _checkTimer(unsigned long &millis, int &interval);
        void _updateClusterBrightness();
        void _updateLedBrightness();
        int _getOffsetLedPosition(int currentPosition);
        int _getClusterLength(int clusterIndex);
        int _getFirstIndexInCluster(int clusterIndex);
        int _getLastIndexInCluster(int clusterIndex);
        bool _isLedInCluster(int ledIndex, int clusterIndex);
};

#endif //ATCSYNCHROTRON_H
