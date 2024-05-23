/*
* Эффект радуги на ATTINY13
* Copyright (C) 2016 Алексей Шихарбеев
* http://samopal.pro
*/

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "light_ws2812.h"

// Количество светодиодов в ле 1нте - 16
#define NUM_PIXEL 7  // колличество светодиодов на плате
#define STEP1 16
#define STEP2 2
#define STEP3 1
#define STEP4 9
#define BTN_PIN A3

byte counter;
uint8_t TM = 10;

struct cRGB led[NUM_PIXEL];
uint8_t num_pixel = NUM_PIXEL;
uint8_t mode = 0;
uint8_t color = 0;
bool pressed = false;
// button port b pin 3
long loopTime = 10000;  // 7 минут
//long loopTime2 = 20000; // 7 минут
//long loopTime3 = 30000; // 7 минут
// pinMode

void setup() {
  // pinMode(BTN_PIN, INPUT_PULLUP);
  DDRB &= ~_BV(PB3);
  PORTB |= _BV(PB3);
}
void loop() {
  // pinMode(BTN_PIN, INPUT_PULLUP);
  if ((PINB & _BV(PB3)) == 0) {
    if (!pressed) {
      mode = (mode + 1) % 3;
    }
    pressed = true;
  } else {
    pressed = false;
  }

  unsigned long currentMillis = millis();
  if (mode == 0) {
    for (int i = 0; i < num_pixel; i++) {
      Wheel(color + i * STEP3, i);
      ws2812_setleds(led, num_pixel);
      color += STEP3;
      delay(TM);
    }
  } else if (mode == 1) {
    for (int i = 0; i < num_pixel; i++) {
      Wheel_1(color + i * STEP1, i);
      ws2812_setleds(led, num_pixel);
      color += STEP1;
      delay(TM);
    }
  } else if (mode == 2) {
    for (int i = 0; i < num_pixel; i++) {
      Wheel_2(color + i * STEP3, i);
      ws2812_setleds(led, num_pixel);
      color += STEP3;
      delay(TM);
    }
  }
}



// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
void Wheel(byte WheelPos, uint8_t n) {
  if (WheelPos < 85) {
    led[n].r = WheelPos * 3;
    led[n].g = 255 - WheelPos * 3;
    led[n].b = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    led[n].r = 255 - WheelPos * 3;
    led[n].g = 0;
    led[n].b = WheelPos;
  } else {
    WheelPos -= 170;
    led[n].r = 0;
    led[n].g = WheelPos * 3;
    led[n].b = 255 - WheelPos * 3;
  }
}


void Wheel_1(byte WheelPos, uint8_t n) {
  if (WheelPos) {
    led[n].r = 255;
    led[n].g = 0;
    led[n].b = 0;
  }
}

void Wheel_2(byte WheelPos, uint8_t n) {

  if (WheelPos < 148) {
    led[n].r = 255 - WheelPos * 3;
    led[n].g = 0;
    led[n].b = 0;
  } else {
    WheelPos -= 170;
    led[n].r = 85 - WheelPos * 1;
    led[n].g = 0;
    led[n].b = 0;
  }
}
