#include "pads.h"
#include "midi.h"
#include "buttons.h"

void setup() {
  pads_init();
  midi_init();
  buttons_init();
}

void loop() {
  pads_update();
  buttons_update();
}
