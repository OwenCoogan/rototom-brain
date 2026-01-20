#pragma once

// Maximum number of piezo pads
#define MAX_PADS 6

// Piezo pad structure
struct PiezoPad {
    int pin;
    int threshold;        // Minimum value to trigger a hit
    int lastValue;         // Last read value
    bool isActive;         // Currently being hit
    unsigned long lastHitTime;  // Time of last hit
    int intensity;         // Current hit intensity (0-127)
};

// Callback function type for pad hits
typedef void (*PadHitCallback)(int padIndex, int intensity);

void pads_init();
void pads_update();
void pads_setHitCallback(PadHitCallback callback);
int pads_getIntensity(int padIndex);
bool pads_isActive(int padIndex);
