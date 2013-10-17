
#include <TimedAction.h>

#define LED1 2
#define LED2 8
#define LED3 9
#define LED4 10

void blink1() {
	static char state = 0;
	if (state)
		state = 0;
	else
		state = 1;
		
	digitalWrite(LED1, state);
}

void blink2() {
	static char state = 0;
	if (state)
		state = 0;
	else
		state = 1;
		
	digitalWrite(LED2, state);
}

void blink3() {
	static char state = 0;
	if (state)
		state = 0;
	else
		state = 1;
		
	digitalWrite(LED3, state);
}

void blink4() {
	static char state = 0;
	if (state)
		state = 0;
	else
		state = 1;
		
	digitalWrite(LED4, state);
}

TimedAction blink1Action = TimedAction(1000, blink1);
TimedAction blink2Action = TimedAction(500, blink2);
TimedAction blink3Action = TimedAction(250, blink3);
TimedAction blink4Action = TimedAction(100, blink4);

void setup() {
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LED3, OUTPUT);
	pinMode(LED4, OUTPUT);
}

void loop() {
	blink1Action.check();
	blink2Action.check();
	blink3Action.check();
	blink4Action.check();
}