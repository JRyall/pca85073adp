#ifndef PCA85073ADP_H
#define PCA85073ADP_H

#include "mbed.h"

//address of the PCA85073ADP IC on the I2C bus
#define I2C_ADDR            0xA2 

// registar overview - crtl & status reg
#define RTC_CTRL_1 			0x00
#define RTC_CTRL_2 			0x01
#define RTC_OFFSET 			0x02
#define RTC_RAM_by 			0x03
// registar overview - time & data reg
#define RTC_SECOND_ADDR		0x04
#define RTC_MINUTE_ADDR		0x05
#define RTC_HOUR_ADDR		0x06
#define RTC_DAY_ADDR		0x07
#define RTC_WDAY_ADDR		0x08
#define RTC_MONTH_ADDR		0x09
#define RTC_YEAR_ADDR		0x0A	// years 0-99
// registar overview - alarm reg
#define RTC_SECOND_ALARM	0x0B
#define RTC_MINUTE_ALARM	0x0C
#define RTC_HOUR_ALARM		0x0D
#define RTC_DAY_ALARM		0x0E
#define RTC_WDAY_ALARM		0x0F

class PCA85073ADP {
public:
    PCA85073ADP(I2C &i2c);

    void setTime(uint8_t hour, uint8_t min, uint8_t sec); //DONE
	void setDate(uint8_t weekday, uint8_t day, uint8_t month, uint8_t yr); //DONE
	char readTime(int n); 
	void enableAlarm();	// called on setAlarm()
	void setAlarm(uint8_t alarm_second, uint8_t alarm_minute, uint8_t alarm_hour, uint8_t alarm_day, uint8_t alarm_weekday);
	char readAlarm(int n);
	bool checkAlarmFlag();
	void resetAlarm();

	/* read RTC times */
	uint8_t getSecond();
	uint8_t getMinute();
	uint8_t getHour();
	uint8_t getDay();
	uint8_t getWeekday();
	uint8_t getMonth();
	uint16_t getYear();

	/* read alarm times */ 			// if return = 99, no alarm
	uint8_t getAlarmSecond();
	uint8_t getAlarmMinute();
	uint8_t getAlarmHour();
	uint8_t getAlarmDay();
	uint8_t getAlarmWeekday();
    
private:
    I2C &_i2c;
    int addr;

    /* time variables */
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t day;
	uint8_t weekday;
	uint8_t month;
	uint16_t year;

	/* alarm */
	uint8_t alarm_second;
	uint8_t alarm_minute;
	uint8_t alarm_hour;
	uint8_t alarm_day;
	uint8_t alarm_weekday;

};

#endif