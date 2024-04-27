#pragma once

#include <stdint.h>
#include "ch554.h"
#include "pwm.h"
#include "delay.h"
#include "gpio.h"
#include "config.h"

void SPEAKER_Init();

void SPEAKER_Generate(uint8_t duration);
