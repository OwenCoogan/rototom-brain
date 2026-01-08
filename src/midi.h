#pragma once

// MIDI channel (1-16, where 1 is the first channel)
#define MIDI_CHANNEL 1

// Default MIDI note for trigger (60 = C4)
#define DEFAULT_MIDI_NOTE 60

void midi_init();
void midi_note_on(int note, int velocity);
void midi_note_off(int note);
void midi_trigger(int intensity);
