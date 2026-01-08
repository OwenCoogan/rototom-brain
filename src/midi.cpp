#include "midi.h"
#include <Arduino.h>

void midi_init() {
  Serial.println("MIDI service initialized (stub)");
}

void midi_note_on(int note, int velocity) {
  Serial.printf("MIDI stub: Note On - note=%d, velocity=%d\n", note, velocity);
}

void midi_note_off(int note) {
  Serial.printf("MIDI stub: Note Off - note=%d\n", note);
}

void midi_trigger(int intensity) {
  Serial.printf("MIDI stub: Trigger called with intensity %d\n", intensity);
  midi_note_on(60, intensity);
  delay(10);
  midi_note_off(60);
}