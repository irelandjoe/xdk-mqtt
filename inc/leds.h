#pragma once

#include "led.h"
#include "retcode.h"

static const int RED_LED = 0;
static const int YELLOW_LED = 1;
static const int ORANGE_LED = 2;
static const int NUM_LEDS = 3;

int LedInit(void);
int LedDeinit(void);
void LedSet(int led, LED_operations_t op);
