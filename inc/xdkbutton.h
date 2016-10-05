#pragma once

#include <stdint.h>
#include <stdio.h>
#include "retcode.h"

static const int32_t BUTTONONE = 1;

typedef void(*buttonHandler)(void*, uint32_t);

XDK_Retcode_E ButtonOneInit(buttonHandler handler);
int IsButtonPressed(void* handle);
int IsButtonReleased(void* handle);
