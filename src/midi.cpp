#include "midi.h"
#include <Arduino.h>
#include <Adafruit_TinyUSB.h>

// USB MIDI instance
Adafruit_USBD_MIDI usb_midi;

void midi_init() {
  // Initialize USB (required for USB MIDI)
  TinyUSBDevice.begin();
  
  // Give USB time to initialize
  delay(500);
  
  Serial.println("\n=== Native USB MIDI Initialized ===");
  Serial.println("Device: ESP32-S3 MIDI Controller");
  Serial.println("Device should appear in GarageBand as 'ESP32-S3 MIDI'");
  Serial.println("No bridge software needed!");
  Serial.println("============================\n");
}

// Send a MIDI Note On message
void midi_note_on(int note, int velocity) {
  // Clamp values to valid MIDI range
  note = constrain(note, 0, 127);
  velocity = constrain(velocity, 0, 127);
  
  // MIDI Note On: 0x90 + channel (0-15), so channel 1 = 0x90
  uint8_t status = 0x90 + (MIDI_CHANNEL - 1);
  
  // Send MIDI message via USB MIDI
  // USB MIDI packet format: [Cable+CIN, Status, Data1, Data2]
  // CIN 0x9 = Note On, CIN 0x8 = Note Off
  uint8_t midi_packet[4] = {
    0x09,           // Cable number (0) and Code Index Number (9 = Note On)
    status,         // MIDI status byte (0x9n)
    note,           // Note number
    velocity        // Velocity
  };
  
  usb_midi.writePacket(midi_packet);
  
  Serial.printf("MIDI: Note On - note=%d, velocity=%d\n", note, velocity);
}

// Send a MIDI Note Off message
void midi_note_off(int note) {
  // Clamp note to valid MIDI range
  note = constrain(note, 0, 127);
  
  // MIDI Note Off: 0x80 + channel (0-15), so channel 1 = 0x80
  uint8_t status = 0x80 + (MIDI_CHANNEL - 1);
  
  // Send MIDI message via USB MIDI
  // USB MIDI packet format: [Cable+CIN, Status, Data1, Data2]
  // CIN 0x8 = Note Off
  uint8_t midi_packet[4] = {
    0x08,           // Cable number (0) and Code Index Number (8 = Note Off)
    status,         // MIDI status byte (0x8n)
    note,           // Note number
    0               // Velocity (0 for note off)
  };
  
  usb_midi.writePacket(midi_packet);
  
  Serial.printf("MIDI: Note Off - note=%d\n", note);
}

void midi_trigger(int intensity) {
  // Intensity is already 0-127 from the slider, map to 1-127 to avoid velocity 0
  int velocity = map(constrain(intensity, 0, 127), 0, 127, 1, 127);
  
  // Trigger a note (default MIDI note 60 = C4)
  midi_note_on(DEFAULT_MIDI_NOTE, velocity);
  delay(10);
  midi_note_off(DEFAULT_MIDI_NOTE);
}