#ifndef PROTONPACKV3_DISPLAYS_H

// Load required Dependencies
#include <GhostLab42Reboot.h>
#include <Wire.h>

#define PROTONPACKV3_DISPLAYS_H
class ATCDisplays {
    public:
        // Constructor
        ATCDisplays();

        // Initialize the Displays
        void init();
        // Call from within loop() to update displays
        void update();
        // Pause the displays so they stop updating
        void pause();
        // Resume the displays after pausing
        void resume();
        // Disable the displays (switch off/blank)
        void disable();
        // Enable the displays (switch on)
        void enable();
        /**
         * @todo
         * Potential Enhancements:
         * - changeSpeed() - Change the speed from 30ms so the displays are faster/slower
         * - disableDisplay(int) - Disable an individual display
         * - pauseDisplay(int) - Pause an individual display
         * - lockDisplay(int,String) - Pause a display while writing a specific fixed value
         */
    private:
        // Holds our GhostLab42 display device instance
        GhostLab42Reboot _device;

        // Check whether the timer has elapsed and we should update
        bool _checkTimer();
        // Write the same value to all 3 displays
        void _writeAll(String value);

        int _topDisplay = 0;
        int _midDisplay = 1;
        int _lowerDisplay = 2;

        bool _paused = false;
        int _lastCount = 0;
        int _currentCount = 0;
        int _count = 2087;

        int _speed = 30;
        unsigned long _previousMillis = 0;
};
#endif //PROTONPACKV3_DISPLAYS_H
