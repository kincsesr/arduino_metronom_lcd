/*
   Simple metronom
*/

#include "pins.h"
#include "state.h"
#include "LiquidCrystal.h"

#define NOTE_HIGH  392
#define NOTE_LOW  196

#define NOTE_DURATION 40
#define TEMPO 120L

#define SECOND 1000
#define MINUTE 60L * SECOND

const int numPatterns = 4;
int pattern[] = {
  1, 0, 0, 0
};
int counter = 0;

long tempo = TEMPO;
long fullNoteDuration = MINUTE / tempo;

State state;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  
  // soros sebesseg
  Serial.begin(9600);
  
  Serial.println("Start...");
  lcd.begin(16,2);

  // initialize buttons;
  pinMode(onOffButtonPin, INPUT);
  pinMode(tempoUpButtonPin, INPUT);
  pinMode(tempoDownButtonPin, INPUT);

  state.enabled = 1;
  state.tempoUp = 0;
  state.tempoDown = 0;
  state.prell_delay = 0;
}

void handle_tempoUp(State* state) {
  // ha lenyomtak a gombot, akkor felvillantjuk a LED-et
  if (state->tempoUp) {
    // noveljuk a tempot
    if (tempo < 200) {
      tempo += 5;
    }
  }
}

void handle_tempoDown(State* state) {
  // ha lenyomtak a gombot, akkor felvillantjuk a LED-et
  if (state->tempoDown) {
    // csokkentjuk a tempot
    if (tempo > 40) {
      tempo -= 5;
    }
  }
}

void handle_enabled(State state) {
  if (state.enabled) {
    counter = (counter + 1) % numPatterns;

    int noteToPlay = pattern[counter] == 1 ? NOTE_HIGH : NOTE_LOW;

    // turn the LED on
    //digitalWrite(rhythmLedPin, HIGH);
    int noteDuration = NOTE_DURATION;

    if (counter == 0) {
      noteDuration -= state.prell_delay;
      //digitalWrite(accentLedPin, HIGH);
    }

    // send the note
    tone(speaker, noteToPlay, noteDuration);

    // turn the LED off
    //digitalWrite(rhythmLedPin, LOW);
    //digitalWrite(accentLedPin, LOW);

    // to distinguish the notes
    long pauseBetweenNotes = fullNoteDuration - NOTE_DURATION;
    delay(pauseBetweenNotes);

    // stop the tone playing:
    noTone(speaker);
  } else {
    noTone(speaker);
    delay(SECOND / 4);
  }

}

void print_state_on_lcd(State state) {
  if (state.tempoUp) {
    lcd.clear();
    lcd.print("tempo up: ");
    lcd.print(tempo);

    lcd.setCursor(0, 1);

    if (tempo == 200) {
      lcd.print("max");
    }
  }
  
  if (state.tempoDown) {
    lcd.clear();
    lcd.print("tempo down: ");
    lcd.print(tempo);

    lcd.setCursor(0, 1);

    if (tempo == 40) {
      lcd.print("min");
    }
  }

}

void clear_state(State* state) {
  state->prell_delay = 0;
  state->tempoUp = 0;
  state->tempoDown = 0; 
}

void loop() {
  get_state(&state);

  handle_tempoUp(&state);
  handle_tempoDown(&state);
  
  fullNoteDuration = MINUTE / tempo;
  
  handle_enabled(state);

  print_state_on_lcd(state);

  clear_state(&state);

}
