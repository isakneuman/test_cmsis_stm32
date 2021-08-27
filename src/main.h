/*
 *
 *
 * stm32f1xx_it.h
 * extern volatile uint32_t ticks_delay;
 *
 * stm32f1xx_t.c
 * volatile uint32_t ticks_delay = 0;
	void SysTick_Handler(void)
	{
		ticks_delay++;
	}
 *
 */

#ifndef __MAIN_H__
#define __MAIN_H__

#define SYSTICK_MAX_VALUE 16777215

#include "stm32f10x.h"
#include "stm32f1xx_it.h"

void init_led(void);
void delay(const uint32_t milliseconds);
void LED_toggle(void);

void click(void);

void init_button(void);

#endif
