
#include "digital_clock.h"
#include"LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd(0x27, 16, 2);

int hour, minute, second, percent_second;
int count_hour, count_minute, count_second;
void lcd_setup() {
	lcd.init();
	lcd.backlight();
	lcd.print("Hello kitty");
	delay(1000);
	lcd.clear();
}
bool button2, button1;
int firstRead1, secondRead1, firstRead2, secondRead2;
int count, count_1s;
bool auto_increase;
bool press1 , press2=false ;
bool change_mode ;
int sw_state=0;
void read_button1() {
	secondRead1 = digitalRead(PIN12);
	firstRead1 = secondRead1;
	secondRead1 = digitalRead(PIN12);
	if (secondRead1 == firstRead1) {
		if (firstRead1 == false) {
			button1 = true;
			press1 = true;
		}
		else {
			button1 = false;
		}
		
		if (!button1&& press1) {
			Serial.println("change mode");
			change_mode = true;
			press1 =false;
		}
		/*else {
			change_mode = false;
		}*/
	}
}
bool increase;
void read_button2() {
	secondRead2 = digitalRead(PIN14);
	firstRead2 = secondRead2;
	secondRead2 = digitalRead(PIN14);
	if (secondRead2==firstRead2) {
		if (firstRead2 == false) {
			button2 = true;
			count++;
			press2 = true;
			if (count >= 100) {
				count_1s++;
				if (count_1s == 50) {
					auto_increase = true;
					count_1s = 0;
				}
			}
		}
		else {
			button2 = false;
			count = 0;
			count_1s = 0;
			auto_increase = false;
		}
		if(!button2&&press2) {
			press2 = false;
			Serial.println("increase");
			increase = true;
		}
	}
}
bool run=false;
int st_minute, st_second, st_percent, count_st_percent;
void stop_watch() {	
	lcd.setCursor(4, 1);
	if (st_minute< 10) {
		lcd.print("0");
	}
	lcd.print(st_minute);
	lcd.print(":");
	if (st_second< 10) {
		lcd.print("0");
	}
	lcd.print(st_second);
	lcd.print(":");
	if (st_percent < 10) {
		lcd.print("0");
	}
	lcd.print(st_percent);
	lcd.print("   ");
}
void run_sw() {
	if(run) st_percent++;
	if (st_percent == 100) {
		st_second++;
		st_percent = 0;
	}
	if (st_second == 60) {
		st_minute++;
		st_second = 0;
	}
	
}
void digital_clock() {	
	percent_second++;
	
	if (percent_second == 100) {
		second++;
		percent_second = 0;
	}
	if (second == 60) {
		minute++;
		second = 0;
	}
	if (minute == 60) {
		hour++;
		minute = 0;
	}
	if (hour == 24) hour = 0;
}
enum { mode0, mode1, mode2, mode3, mode4 }Mode;
bool blinky;
void display_digital_clock() {
	
	lcd.setCursor(4, 1);
	if(!blinky) {
		if (hour < 10) {
			lcd.print("0");
		}
		lcd.print(hour);
		lcd.print(":");
		if (minute < 10) {
			lcd.print("0");
		}
		lcd.print(minute);
		lcd.print(":");
		if (second < 10) {
			lcd.print("0");
		}
		lcd.print(second);
		lcd.print("    ");
	}
	else {
		if(Mode==mode1) {
			if(count_hour>=30) {
				if (hour < 10) {
					lcd.print("0");
				}
				lcd.print(hour);
			}
			else {
				lcd.print("  ");
			}
			lcd.print(":");
			if (minute < 10) {
				lcd.print("0");
			}
			lcd.print(minute);
			lcd.print(":");
			if (second < 10) {
				lcd.print("0");
			}
			lcd.print(second);
			lcd.print("    ");
		}
		if(Mode==mode2){
			if (hour < 10) {
				lcd.print("0");
			}
			lcd.print(hour);
			lcd.print(":");
			if(count_minute>=30) {
				if (minute < 10) {
					lcd.print("0");
				}
				lcd.print(minute);
			}
			else {
				lcd.print("  ");
			}
			lcd.print(":");
			if (second < 10) {
				lcd.print("0");
			}
			lcd.print(second);
			lcd.print("    ");

		}
		if(Mode==mode3) {
			if (hour < 10) {
				lcd.print("0");
			}
			lcd.print(hour);
			lcd.print(":");
			if (minute < 10) {
				lcd.print("0");
			}
			lcd.print(minute);
			lcd.print(":");
			if(count_second>=30) {
				if (second < 10) {
					lcd.print("0");
				}
				lcd.print(second);
				lcd.print("    ");
			}
			else {
				lcd.print("  ");
			}
		}
	}
	

}

void fsm() {
	switch (Mode) {
	case mode0:
		lcd.setCursor(5, 0);
		lcd.print("hi:DUY");
		display_digital_clock();
		if (change_mode) {
			Mode = mode1;
			change_mode = false;
			Serial.println("mode0");
			lcd.setCursor(0, 0);
			lcd.print("                ");
		}
		
		blinky = false;
		break;
	case mode1:
		lcd.setCursor(2, 0);
		lcd.print("modidy hours");
		count_hour++;
		if(count_hour==60) {
			count_hour = 0;
		}
		display_digital_clock();
		if (button2) {
			if (auto_increase) {
				hour++;
				auto_increase = false;
			}
			else if(increase) {				
				hour++;
			
			}
			increase = false;
		}
		if (change_mode) {
			Mode = mode2;
			change_mode = false;
			Serial.println("mode1");
			lcd.setCursor(0, 0);
			lcd.print("                ");
		}
		blinky = true;
		break;
	case mode2:
		lcd.setCursor(1,0);
		lcd.print("modify minutes");
		count_minute++;
		if(count_minute==60){
			count_minute=0;
		}
		display_digital_clock();
		if (increase) {
			if (auto_increase) {
				minute++;
				auto_increase = false;
			}
			else minute++;
			increase = false;
		}
		if (change_mode) {
			Mode = mode3;
			change_mode = false;
			Serial.println("mode2");
			lcd.setCursor(0, 0);
			lcd.print("                ");
		}
		blinky = true;
		break;
	case mode3:
		
		lcd.setCursor(1, 0);
		lcd.print("modify seconds");
		count_second++;
		if(count_second==60) {
			count_second = 0;
		}
		display_digital_clock();
		if (increase) {
			if (auto_increase) {
				second++;
				auto_increase = false;
			}
			else second++;
			increase = false;
		}
		if (change_mode) {
			Mode = mode4;
			change_mode = false;
			Serial.println("mode3");
			lcd.setCursor(0, 0);
			lcd.print("                ");
		}
		blinky = true;
		break;
	case mode4:
		lcd.setCursor(3, 0);
		lcd.print("stop watch");
		stop_watch();
		if(increase) {
			sw_state++;
			increase = false;
		}
		if(sw_state ==1) run=true;
		if (sw_state == 2) run = false;
		if(sw_state ==3) {
			st_minute = 0;
			st_percent = 0;
			st_second = 0;
			sw_state = 0;
			
		}
		if (change_mode) {
			Mode = mode0;
			change_mode = false;
			Serial.println("mode4");
			lcd.setCursor(0, 0);
			lcd.print("                ");
		}
		blinky = false;
		break;
	default:
		break;
	}
		
}
	
