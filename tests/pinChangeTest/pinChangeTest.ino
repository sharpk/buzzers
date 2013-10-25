// You can reduce the memory footprint of this handler by declaring that there will be no pin change interrupts
// on any one or two of the three ports.  If only a single port remains, the handler will be declared inline
// reducing the size and latency of the handler.
#define NO_PORTB_PINCHANGES // to indicate that port b will not be used for pin change interrupts
#define NO_PORTC_PINCHANGES // to indicate that port c will not be used for pin change interrupts
// #define NO_PORTD_PINCHANGES // to indicate that port d will not be used for pin change interrupts
// *** New in version 1.5: ********************************
// You can reduce the code size by 20-50 bytes, and you can speed up the interrupt routine
// slightly by declaring that you don't care if the static variables PCintPort::pinState and/or
// PCintPort::arduinoPin are set and made available to your interrupt routine.
#define NO_PIN_STATE        // to indicate that you don't need the pinState
// #define NO_PIN_NUMBER       // to indicate that you don't need the arduinoPin
// *********************************************************
// if there is only one PCInt vector in use the code can be inlined
// reducing latency and code size
// define DISABLE_PCINT_MULTI_SERVICE below to limit the handler to servicing a single interrupt per invocation.
// #define       DISABLE_PCINT_MULTI_SERVICE
//-------- define the above in your sketch, if applicable ------------------------------------------------------
#include <PinChangeInt.h>

#define NUM_INPUTS 4
#define INPUT1_PIN 7
#define INPUT2_PIN 6
#define INPUT3_PIN 5
#define INPUT4_PIN 4
#define RESET_PIN 3

unsigned long times[NUM_INPUTS];
const char pinMap[] = {INPUT1_PIN, INPUT2_PIN, INPUT3_PIN, INPUT4_PIN};

void pcintFunc() {
	times[7 - PCintPort::arduinoPin] = millis();
}

void reset() {
	int i;
	for (i=0; i<NUM_INPUTS; i++)
		times[i] = 0;
		
	for (i=0; i<NUM_INPUTS; i++) {
		pinMode(pinMap[i], INPUT); digitalWrite(pinMap[i], HIGH);
		PCintPort::attachInterrupt(pinMap[i], &pcintFunc, FALLING);
	}
}

void setup() {
	reset();
	
	Serial.begin(9600);
	Serial.println("Pin Change Interrupt test started...");
}

void loop() {
	int i;
	for (i = 0; i < NUM_INPUTS; i++) {
		if (times[i]) {
			Serial.print("Input ");
			Serial.print(i);
			Serial.print(" time = ");
			Serial.println(times[i]);
			times[i] = 0;
		}
	}
}
