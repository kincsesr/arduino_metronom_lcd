#ifndef STATE
#define STATE

typedef struct State {
  int enabled;   // on/off enabled/disabled
  int tempoUp;
  int tempoDown; 
  int prell_delay; // sum of delay because of prell
} State;


void get_state(State* state);

#endif
