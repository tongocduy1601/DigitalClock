// digital_clock.h

#ifndef _DIGITAL_CLOCK_h
#define _DIGITAL_CLOCK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#define PIN12 12
#define PIN14 14
void read_button1();
void read_button2();
void stop_watch();
void digital_clock();
void display_digital_clock();
void fsm();
void run_sw();
void lcd_setup();
#endif

