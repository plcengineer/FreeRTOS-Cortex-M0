/*
 * helper.cpp
 *
 *  Created on: 08.02.2016
 *      Author: Arndt
 */
#include "helper.h"

const char* short_weekdays[7] = { "Mon.", "Tue.", "Wed.", "Thu.", "Fri.",
		"Sat.", "Sun." };

long timespan(uint32_t t1, uint32_t t2) {
	long result;
	result = (t2 < t1) ? /* overflow? */
	pow(2, 31) - t1 + t2 : t2 - t1;
	return result;
}

void rt_delayMicroseconds(uint32_t usec) {

	vTaskDelay(usec / portTICK_PERIOD_US);
}

uint32_t rt_micros(void) {
	return xTaskGetTickCount() * portTICK_PERIOD_US;
}

uint32_t rt_millis(void) {
	return (xTaskGetTickCount() * portTICK_PERIOD_US) / 1000;
}

void rt_delay(uint32_t ms) {
	vTaskDelay((ms * 1000) / portTICK_PERIOD_US);
}

void rtos_run() {
	delay = &rt_delay;
	micros = &rt_micros;
	millis = &rt_millis;
	delayMicroseconds = &rt_delayMicroseconds;
	vTaskStartScheduler();
}

extern void (*rtosSysTick_Handler)(void);
void rtos_stop() {
	portENTER_CRITICAL();
	rtosSysTick_Handler = NULL;
	delay = &_delay;
	micros = &_micros;
	millis = &_millis;
	delayMicroseconds = &_delayMicroseconds;
	portEXIT_CRITICAL();
}

static SystemClock_t sysClock;
volatile static TickType_t _lastTickCount = 0L;
SystemClock_t updateSysClock(void) {
	portENTER_CRITICAL();
	long tickspan = timespan(_lastTickCount, xTaskGetTickCount());
	_lastTickCount += tickspan;

	tickspan *= portTICK_PERIOD_US; // µs
	tickspan += sysClock.micro;
	sysClock.micro = tickspan % 1000000;

	tickspan /= 1000000; // s
	tickspan += sysClock.second;
	sysClock.second = tickspan % 60;

	tickspan /= 60; // min.
	tickspan += sysClock.minute;
	sysClock.minute = tickspan % 60;

	tickspan /= 60; // h
	tickspan += sysClock.hour;
	sysClock.hour = tickspan % 24;

	tickspan /= 24; // days
	tickspan += sysClock.weekday;
	sysClock.weekday = tickspan % 7;

	SystemClock_t result = sysClock; // return a copy of origin sys clock
	portEXIT_CRITICAL();
	return result;
}

void setSysClock(const SystemClock_t* clock_values) {
	portENTER_CRITICAL();
	memcpy(&sysClock, clock_values, sizeof(SystemClock_t));
	_lastTickCount = xTaskGetTickCount();
	portEXIT_CRITICAL();
}

size_t timestr(char* str, const SystemClock_t* time, bool opt_weekday,
bool opt_micros, bool opt_s) {
	return sprintf(str, "%s %02i:%02i:%02i (%06luus)",
			short_weekdays[time->weekday], time->hour, time->minute,
			time->second, time->micro);
}

