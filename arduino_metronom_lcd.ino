/*
   Simple metronom
*/

#include "pins.h"
#include "state.h"
#include "LiquidCrystal.h"

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

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

void setup() {

  //Serial.begin(9600);
  // soros sebesseg
  //Serial.println("Start...");
  lcd.begin(16,2);

  // initialize LEDs
  //pinMode(tempoLedPinUp, OUTPUT);
  //pinMode(tempoLedPinDown, OUTPUT);
  //pinMode(rhythmLedPin, OUTPUT);
  //pinMode(accentLedPin, OUTPUT);

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
    lcd.setCursor(0, 0);
    lcd.print("tempo up pressed");
    //digitalWrite(tempoLedPinUp, HIGH);
    delay(10);
    state->prell_delay += 10;
    //digitalWrite(tempoLedPinUp, LOW);

    // visszaallitjuk az allapotot
    state->tempoUp = 0;

    // noveljuk a tempot
    if (tempo < 200) {
      tempo += 5;
      lcd.setCursor(0, 1);
      lcd.print("tempo is ");
      lcd.println(tempo);
    } else {
      lcd.setCursor(0, 1);
      lcd.print("tempo reached maximum");
    }
  }
}

void handle_tempoDown(State* state) {
  // ha lenyomtak a gombot, akkor felvillantjuk a LED-et
  if (state->tempoDown) {
    lcd.setCursor(0, 0);
    lcd.print("tempo down pressed");
    //digitalWrite(tempoLedPinDown, HIGH);
    delay(10);
    state->prell_delay += 10;
    //digitalWrite(tempoLedPinDown, LOW);

    // visszaallitjuk az allapotot
    state->tempoDown = 0;

    // csokkentjuk a tempot
    if (tempo > 40) {
      tempo -= 5;
      lcd.setCursor(0, 1);
      lcd.print("tempo is ");
      lcd.print(tempo);
    } else {
      lcd.setCursor(0, 1);
      lcd.print("tempo reached minimum");
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

void loop() {
  

  state.prell_delay = 0;
  get_state(&state);

  handle_tempoUp(&state);
  handle_tempoDown(&state);
  
  fullNoteDuration = MINUTE / tempo;
  
  handle_enabled(state);

}
