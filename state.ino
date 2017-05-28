
#include "pins.h"
#include "state.h"

const int prellDelay = 5;

int isHigh(int pin) {
	return digitalRead(pin) == HIGH;
}

int log_on_serial(int pin, char* msg, int state) {
	Serial.print(pin, DEC);
  Serial.print(msg);
  Serial.println(state, DEC);
}

void readButton(int pin, int* state, int* prell_delay) {
	if (isHigh(pin)) {
      delay(prellDelay);
      *prell_delay += prellDelay;
	  
      if (isHigh(pin)) {
        *state = !*state;
        log_on_serial(pin, " button pressed ", *state);
      }
	}
}

void get_state(State* state) {
	// check if the pushbutton is pressed.
	readButton(onOffButtonPin, &state->enabled, &state->prell_delay);
	readButton(tempoUpButtonPin, &state->tempoUp, &state->prell_delay);
	readButton(tempoDownButtonPin, &state->tempoDown, &state->prell_delay);
}
