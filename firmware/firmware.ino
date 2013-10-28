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
#include <TimedAction.h>

#define DBG

#define NUM_BUZZERS 4
#define BUZZER1_PIN 7
#define BUZZER2_PIN 6
#define BUZZER3_PIN 5
#define BUZZER4_PIN 4
#define RESET_PIN 3

#define LED_ON 1
#define LED_OFF 0
#define LED1 2
#define LED2 8
#define LED3 9
#define LED4 10
#define BUZZER_LED1 A0
#define BUZZER_LED2 A1
#define BUZZER_LED3 A2
#define BUZZER_LED4 A3

char counter;
char resetFlag;
char first;
const char buzzerPinMap[] = {BUZZER1_PIN, BUZZER2_PIN, BUZZER3_PIN, BUZZER4_PIN};
const char ledPinMap[] = {LED1, LED2, LED3, LED4, BUZZER_LED1, BUZZER_LED2, BUZZER_LED3, BUZZER_LED4};
TimedAction* buzzerActionMap[NUM_BUZZERS];
char pinBuzzerMap[14];

void blink1() {
	static char state = LED_OFF;
	if (state == LED_ON)
		state = LED_OFF;
	else
		state = LED_ON;
	
	if (first == 1)
		digitalWrite(BUZZER_LED1, state);
	else
		digitalWrite(LED1, state);
}

void blink2() {
	static char state = LED_OFF;
	if (state == LED_ON)
		state = LED_OFF;
	else
		state = LED_ON;

	if (first == 2)
		digitalWrite(BUZZER_LED2, state);
	else
		digitalWrite(LED2, state);
}

void blink3() {
	static char state = LED_OFF;
	if (state == LED_ON)
		state = LED_OFF;
	else
		state = LED_ON;
		
	if (first == 3)
		digitalWrite(BUZZER_LED3, state);
	else		
		digitalWrite(LED3, state);
}

void blink4() {
	static char state = LED_OFF;
	if (state == LED_ON)
		state = LED_OFF;
	else
		state = LED_ON;
		
	if (first == 4)
		digitalWrite(BUZZER_LED4, state);
	else		
		digitalWrite(LED4, state);
}

TimedAction blink1Action = TimedAction(1000, blink1);
TimedAction blink2Action = TimedAction(1000, blink2);
TimedAction blink3Action = TimedAction(1000, blink3);
TimedAction blink4Action = TimedAction(1000, blink4);

void pcintBuzzFunc() {
	char tmpCnt, buzzer;
	PCintPort::detachInterrupt(PCintPort::arduinoPin);
	tmpCnt = counter++;
	buzzer = pinBuzzerMap[PCintPort::arduinoPin];
#ifdef DBG
	Serial.print((int)tmpCnt);
	Serial.print(" ");
	Serial.print((int)counter);
	Serial.print(" ");
	Serial.println(PCintPort::arduinoPin);
#endif
	switch (tmpCnt) {
		case 0:
			first = buzzer+1;
			digitalWrite(ledPinMap[buzzer], LED_ON);
			buzzerActionMap[buzzer]->setInterval(250);
			buzzerActionMap[buzzer]->enable();
			break;
		case 1:
			buzzerActionMap[buzzer]->setInterval(100);
			buzzerActionMap[buzzer]->enable();
			break;
		case 2:
			buzzerActionMap[buzzer]->setInterval(500);
			buzzerActionMap[buzzer]->enable();
			break;
		default:
			digitalWrite(ledPinMap[buzzer], LED_OFF);
			break;
	}
}

void pcintResetFunc() {
	resetFlag = 1;
}

void reset() {
	int i;
	counter = 0;
	resetFlag = 0;
	first = 0;
	
	for (i=0; i<NUM_BUZZERS; i++)
		buzzerActionMap[i]->disable();
	
	for (i=0; i< NUM_BUZZERS*2; i++)
		digitalWrite(ledPinMap[i], LED_OFF);
	
	for (i=0; i<NUM_BUZZERS; i++) {
		pinMode(buzzerPinMap[i], INPUT); digitalWrite(buzzerPinMap[i], HIGH);
		PCintPort::attachInterrupt(buzzerPinMap[i], &pcintBuzzFunc, FALLING);
	}
}

void setup() {
	int i;
	
	Serial.begin(9600);
	Serial.println("Initializing Buzzers...");
	
	for (i=0; i< NUM_BUZZERS*2; i++)
		pinMode(ledPinMap[i], OUTPUT);
	
	for (i=0; i<14; i++)
		pinBuzzerMap[i] = -1;
	
	for (i=0; i<NUM_BUZZERS; i++)
		pinBuzzerMap[buzzerPinMap[i]] = i;
	
	buzzerActionMap[0] = &blink1Action;
	buzzerActionMap[1] = &blink2Action;
	buzzerActionMap[2] = &blink3Action;
	buzzerActionMap[3] = &blink4Action;
	
	reset();
	
	pinMode(RESET_PIN, INPUT); digitalWrite(RESET_PIN, HIGH);
	PCintPort::attachInterrupt(RESET_PIN, &pcintResetFunc, FALLING);
	
	Serial.println("Buzzers Started...");
}

void loop() {
	int i;
	for (i=0; i<NUM_BUZZERS; i++) {
		buzzerActionMap[i]->check();
		
		if (resetFlag)
			reset();
	}
}

