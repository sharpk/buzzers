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

#define NUM_BUZZERS 4
#define BUZZER1_PIN 7
#define BUZZER2_PIN 6
#define BUZZER3_PIN 5
#define BUZZER4_PIN 4
#define RESET_PIN 3

// Add LED pin defs here

char counter;
char resetFlag;
char blinkState[NUM_BUZZERS];
const char buzzerPinMap[] = {BUZZER1_PIN, BUZZER2_PIN, BUZZER3_PIN, BUZZER4_PIN};
char pinBuzzerMap[14];

// Add LED TimedAction objects here

void pcintBuzzFunc() {
	PCintPort::detachInterrupt(PCintPort::arduinoPin);
	blinkState[pinBuzzerMap[PCintPort::arduinoPin]] = counter;
	counter++;
}

void pcintResetFunc() {
	resetFlag = 1;
}

void reset() {
	int i;
	counter = 0;
	resetFlag = 0;
	
	for (i=0; i<NUM_BUZZERS; i++)
		blinkState[i] = 0;
		
	for (i=0; i<NUM_BUZZERS; i++) {
		pinMode(buzzerPinMap[i], INPUT); digitalWrite(buzzerPinMap[i], HIGH);
		PCintPort::attachInterrupt(buzzerPinMap[i], &pcintBuzzFunc, FALLING);
	}
}

void setup() {
	int i;
	
	Serial.begin(9600);
	Serial.println("Initializing Buzzers...");
	
	// Init LEDs here
	
	for (i=0; i<14; i++)
		pinBuzzerMap[i] = -1;
	
	for (i=0; i<NUM_BUZZERS; i++)
		pinBuzzerMap[buzzerPinMap[i]] = i;

	reset();
	
	pinMode(RESET_PIN, INPUT); digitalWrite(RESET_PIN, HIGH);
	PCintPort::attachInterrupt(RESET_PIN, &pcintResetFunc, FALLING);
	
	Serial.println("Buzzers Started...");
}

void loop() {
	// go through all the buzzers and blink the right pattern on LEDs


	if (resetFlag) {
		reset();
	}
}

