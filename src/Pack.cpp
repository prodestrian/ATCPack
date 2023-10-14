#include <Arduino.h>
#include "ATCDisplays/ATCDisplays.h"

ATCDisplays displays;

void setup() {
    displays.init();
}

void loop() {
    displays.update();
}
