#include "midi.h"
#include <USB.h>
#include <USBMIDI.h>

USBMIDI MIDI;

void midi_init() {
  MIDI.begin();
  USB.begin();
}

void midi_note_on(int note, int velocity) {
  MIDI.noteOn(note, velocity, 1);
}

void midi_note_off(int note) {
  MIDI.noteOff(note, 0, 1);
}