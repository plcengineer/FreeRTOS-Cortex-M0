/*
 * helper.h
 *
 *  Created on: 08.02.2016
 *      Author: Arndt
 */

#ifndef HELPER_H_
#define HELPER_H_

#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>
#include <delay.h>

long timespan(uint32_t t1, uint32_t t2);

void rtos_run(void);

void rtos_stop(void);

struct SystemClock_t {
	uint8_t weekday; // 0 = Monday, ... 6 = Sunday
	uint8_t hour;   // 0.. 23 h
	uint8_t minute; // 0.. 59 m
	uint8_t second; // 0.. 59 s
	uint32_t micro; // 0.. 999999 us
};

SystemClock_t updateSysClock(void);
void setSysClock(const SystemClock_t* clock_values);

size_t timestr(char* str, const SystemClock_t* time, bool opt_weekday,
		bool opt_micros, bool opt_s);

#endif /* HELPER_H_ */
