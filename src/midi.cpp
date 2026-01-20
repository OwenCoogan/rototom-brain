#include "midi.h"
#include <Arduino.h>
#include <USB.h>
// #include <USBMIDI.h>

// USBMIDI MIDI;

void midi_init() {
  // MIDI.begin();
  // USB.begin();
}

void midi_note_on(int note, int velocity) {
  // MIDI.noteOn(note, velocity, 1);
  Serial.printf("MIDI: Note On - note=%d, velocity=%d\n", note, velocity);
}

void midi_note_off(int note) {
  // MIDI.noteOff(note, 0, 1);
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