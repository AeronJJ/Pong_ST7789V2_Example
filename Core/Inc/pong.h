#ifndef PONG_H
#define PONG_H

#include "LCD.h"
#include "stm32l4xx_hal.h"


void pong_init();
uint8_t pong_run(ST7789V2_cfg_t* cfg);

#endif