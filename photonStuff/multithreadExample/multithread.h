#include "application.h"

// LED Param, defines pin and delay between blinks
typedef struct {
    int pin;
    int delay;
} LED_PARAM;

// LED pins
int led1 = D5;
int led2 = D3;
int led3 = D1;

// Defines thread parameters
LED_PARAM ledParams[3] = {
    {led1, 500},
    {led2, 1000},
    {led3, 2000}
};

Thread* led1Thread;
Thread* led2Thread;
Thread* led3Thread;

os_thread_return_t ledBlink(void* param){
    LED_PARAM *p = (LED_PARAM*)param;

    // Start never ending loop
    for(;;) {
        Serial.print("Thread Parameters: ");
        Serial.print(p->pin);
        Serial.print(", ");
        Serial.println(p->delay);

        // Blink led
        digitalWrite(p->pin, HIGH);
        delay(p->delay);
        digitalWrite(p->pin, LOW);
        delay(p->delay);
    }
}

void setup() {
    Serial.begin(115200);

    // Set pin mode
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);

    // Start new threads
    led1Thread = new Thread("sample", ledBlink, &ledParams[0]);
    led2Thread = new Thread("sample", ledBlink, &ledParams[1]);
    led3Thread = new Thread("sample", ledBlink, &ledParams[2]);
}

void loop() {
    // Nothing here.
}
