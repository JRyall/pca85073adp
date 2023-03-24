#include "mbed.h"
#include "PCA85073ADP.h"

//Pin Defs //
DigitalOut LED(PA_5);                   // LED

////////////////////////////////////////////////RTC OVER I2C/////////////////////////////////////////
//define the I2C Pins
I2C i2c(PB_7, PB_6);
// initialize RTC object
PCA85073ADP rtc(i2c); 
////////////////////////////////////////////////RTC OVER I2C/////////////////////////////////////////

void led()
{
    LED = !LED;
}

// main() runs in its own thread in the OS
int main()
{
    ////////////////////////////////////////////////RTC OVER I2C/////////////////////////////////////////
    rtc.resetAlarm(); //clears the flags for the inturrupt and the alarm triggered flag
    rtc.setTime(13, 02, 55); //Set the time (Hours, Mins, Secs)
    rtc.setDate(1, 21, 06, 23); //Set the date (weekday, day, month, year(0-99))
    rtc.setAlarm(13, 03, 0, 21, 1); //Set the alarm (hour, minute, second, day, weekday)
    ////////////////////////////////////////////////RTC OVER I2C/////////////////////////////////////////

    //run the tasks defined in the delayed threads
    while (true) {
        led();
        printf("Time: ");
        printf("%d:%d:%d ", rtc.getHour(), rtc.getMinute(), rtc.getSecond());
        printf("Date: ");
        if(rtc.getWeekday() == 0){ printf("Sunday ");}
        if(rtc.getWeekday() == 1){ printf("Monday ");}
        if(rtc.getWeekday() == 2){ printf("Tuesday ");}
        if(rtc.getWeekday() == 3){ printf("Wednesday ");}
        if(rtc.getWeekday() == 4){ printf("Thursday ");}
        if(rtc.getWeekday() == 5){ printf("Friday ");}
        if(rtc.getWeekday() == 6){ printf("Saturday ");}
        printf("the %d:%d:%d ", rtc.getDay(), rtc.getMonth(), rtc.getYear());
        printf("\r\n");
        printf("\r\n");
        printf("Alarm Time: %d:%d:%d", rtc.getAlarmHour(), rtc.getAlarmMinute(), rtc.getAlarmSecond());
        printf("\r\n");
        printf("Alarm Date: ");
        if(rtc.getAlarmWeekday() == 0){ printf("Sunday ");}
        if(rtc.getAlarmWeekday() == 1){ printf("Monday ");}
        if(rtc.getAlarmWeekday() == 2){ printf("Tuesday ");}
        if(rtc.getAlarmWeekday() == 3){ printf("Wednesday ");}
        if(rtc.getAlarmWeekday() == 4){ printf("Thursday ");}
        if(rtc.getAlarmWeekday() == 5){ printf("Friday ");}
        if(rtc.getAlarmWeekday() == 6){ printf("Saturday ");}
        printf("the %d", rtc.getAlarmDay());
        printf("\r\n");
        printf("Alarm Flag: ");
        printf("%d", rtc.checkAlarmFlag());
        printf("\r\n");
        printf("\r\n");
        printf("\r\n");
        printf("\r\n");
        printf("\r\n");
        printf("\r\n");
        printf("\033[0H\033[0J");
        ThisThread::sleep_for(100ms);
    }
}