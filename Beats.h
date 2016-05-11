#include "Definitions.h"

#ifndef BEATS_H_
#define BEATS_H_

/*
* Fields:
* duration: Duration between two consective taps in clock cycles
* servo_num: Servo 1 or Servo 2
* next: The next beat in linked list
*/
typedef struct beat{
	int hit_time;
	int servo_num;
	struct beat* next;
} beat_t;

void add_to_tail(beat_t** head_ref, beat_t* b);
void setup_beat_timer(void);
void prompt_for_input(void);
beat_t* create_beat(int servo, int time);

#endif //BEATS_H_
