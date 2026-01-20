#include "pads.h"
#include "pins.h"
#include "pages.h"
#include <Arduino.h>

// Piezo pad instances
static PiezoPad pads[MAX_PADS] = {
    {PIEZO_TOM1_PIN, 50, 0, false, 0, 0},  // Tom 1 on GPIO1
    {PIEZO_TOM2_PIN, 50, 0, false, 0, 0},  // Tom 2 on GPIO2
    {PIEZO_TOM3_PIN, 50, 0, false, 0, 0},  // Tom 3 on GPIO3
    {PIEZO_TOM4_PIN, 50, 0, false, 0, 0},  // Tom 4 on GPIO4
    {PIEZO_TOM5_PIN, 50, 0, false, 0, 0},  // Tom 5 on GPIO5
    {PIEZO_TOM6_PIN, 50, 0, false, 0, 0},  // Tom 6 on GPIO6
};

static PadHitCallback hitCallback = nullptr;

// Constants for piezo reading
#define PIEZO_DEBOUNCE_MS 10      // Minimum time between hits (ms)
#define PIEZO_ACTIVE_TIMEOUT 50   // Time to keep pad active after hit (ms)
#define ADC_MAX 4095               // ESP32-S3 ADC max value (12-bit)
#define MIDI_VELOCITY_MAX 127     // Maximum MIDI velocity

void pads_init() {
    Serial.println("Initializing piezo pads...");
    
    // Initialize ADC pins - ONLY initialize GPIO1-3 to avoid conflicts
    // GPIO4=LCD_RST, GPIO5=TOUCH_SCL, GPIO6=TOUCH_SDA - DO NOT TOUCH!
    for (int i = 0; i < MAX_PADS; i++) {
        // Only initialize pads 0-2 (GPIO1-3), skip pads 3-5 to avoid conflicts
        if (i < 3) {
            pinMode(pads[i].pin, INPUT);
            pads[i].lastValue = analogRead(pads[i].pin);
            Serial.printf("Pad %d initialized on GPIO%d\n", i + 1, pads[i].pin);
        } else {
            // Pads 4-6 conflict with display/touch pins - disable them
            Serial.printf("Pad %d skipped (GPIO%d conflicts with display/touch)\n", i + 1, pads[i].pin);
        }
    }
    
    Serial.println("Piezo pads initialized (only GPIO1-3 active)");
}

void pads_setHitCallback(PadHitCallback callback) {
    hitCallback = callback;
}

int pads_getIntensity(int padIndex) {
    if (padIndex < 0 || padIndex >= MAX_PADS) return 0;
    return pads[padIndex].intensity;
}

bool pads_isActive(int padIndex) {
    if (padIndex < 0 || padIndex >= MAX_PADS) return false;
    return pads[padIndex].isActive;
}

void pads_update() {
    unsigned long currentTime = millis();
    
    for (int i = 0; i < MAX_PADS; i++) {
        int currentValue = analogRead(pads[i].pin);
        
        // Check if enough time has passed since last hit (debounce)
        bool canTrigger = (currentTime - pads[i].lastHitTime) >= PIEZO_DEBOUNCE_MS;
        
        // Detect hit: value exceeds threshold and we're not already active
        if (canTrigger && !pads[i].isActive && currentValue > pads[i].threshold) {
            // Calculate intensity (map ADC value to MIDI velocity 0-127)
            // Use a more sensitive mapping for better dynamic range
            int rawIntensity = map(currentValue, pads[i].threshold, ADC_MAX, 0, MIDI_VELOCITY_MAX);
            pads[i].intensity = constrain(rawIntensity, 1, MIDI_VELOCITY_MAX);
            
            pads[i].isActive = true;
            pads[i].lastHitTime = currentTime;
            
            Serial.printf("Pad %d hit! Intensity: %d (raw: %d)\n", i + 1, pads[i].intensity, currentValue);
            
            // Call callback if set
            if (hitCallback) {
                hitCallback(i, pads[i].intensity);
            }
        }
        
        // Deactivate pad after timeout
        if (pads[i].isActive && (currentTime - pads[i].lastHitTime) >= PIEZO_ACTIVE_TIMEOUT) {
            pads[i].isActive = false;
            pads[i].intensity = 0;
            // Reset button color when pad becomes inactive
            pages_resetButtonColor(i);
        }
        
        pads[i].lastValue = currentValue;
    }
}