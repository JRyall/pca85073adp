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
    rtc.setAlarm(13, 03, 0, 21, 1, 1); //Set the alarm (hour, minute, second, day, weekday, interrupt)
    ////////////////////////////////////////////////RTC OVER I2C/////////////////////////////////////////

    //run the tasks defined in the delayed threads
    while (true) {
        led();
        printf("Time: ");
        int* readTime = rtc.readTime();
        printf("%d:%d:%d ", readTime[2], readTime[1], readTime[0]);
        printf("Date: ");
        if(readTime[4] == 0){ printf("Sunday ");}
        if(readTime[4] == 1){ printf("Monday ");}
        if(readTime[4] == 2){ printf("Tuesday ");}
        if(readTime[4] == 3){ printf("Wednesday ");}
        if(readTime[4] == 4){ printf("Thursday ");}
        if(readTime[4] == 5){ printf("Friday ");}
        if(readTime[4] == 6){ printf("Saturday ");}
        printf("the %d:%d:%d ", readTime[3], readTime[5], readTime[6]);
        printf("\r\n");
        printf("\r\n");
        int* readAlarm = rtc.readAlarm();
        printf("Alarm Time: %d:%d:%d", readAlarm[2], readAlarm[1], readAlarm[0]);
        printf("\r\n");
        printf("Alarm Date: ");
        if(readAlarm[4] == 0){ printf("Sunday ");}
        if(readAlarm[4] == 1){ printf("Monday ");}
        if(readAlarm[4] == 2){ printf("Tuesday ");}
        if(readAlarm[4] == 3){ printf("Wednesday ");}
        if(readAlarm[4] == 4){ printf("Thursday ");}
        if(readAlarm[4] == 5){ printf("Friday ");}
        if(readAlarm[4] == 6){ printf("Saturday ");}
        printf("the %d", readAlarm[3]);
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