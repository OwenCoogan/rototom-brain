#pragma once

void midi_init();
void midi_note_on(int note, int velocity);
void midi_note_off(int note);
void midi_trigger(int intensity);
