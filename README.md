# pca85073adp
## Library for use with mbed OS and the pca85073adp RTC
### Written by James Ryall 24/03/2023.

Adding this library allows you to read and write to the PCA85073ADP RTC over i2c in Mbed OS. 

Within this library are the following defines:

>Define the I2C Pins:

I2C i2c(PB_7, PB_6)

>Initialize RTC object: 

PCA85073ADP rtc(i2c)                                      


>Set the time:

setTime(Hour, Minute, Second)       

>Set the date:

setDate(Weekday, Day, Month, Year)

>Set the alarm:

setAlarm(Hour, Minute, Second, Day, Weekday, interrupt)

>Check the alarm flag:

checkAlarmFlag()

>clears the flags for the inturrupt and the alarm triggered flag:

resetAlarm()                                              


>Returns an array that allows you to output all the time data in a singla array, speeding up the i2c comms: 

readTime()


>Returns an array that allows you to output all the alarm data in a singla array, speeding up the i2c comms: 
readAlarm()    


>All single functions self defining, returns an int: 

getHour()
getMinute()
getSecond()
getWeekday()
getDay()
getMonth()
getYear()
getAlarmHour()
getAlarmMinute()
getAlarmSecond()
getAlarmWeekday()
getAlarmDay()


>Example:

```
I2C i2c(PB_7, PB_6)
PCA85073ADP rtc(i2c)
int* readTime = rtc.readTime();


//returns:
readTime[0] // Seconds
readTime[1] // Minutes
readTime[2] // Hours
readDime[3] // Days
readTime[4] // Weekdays
readTime[5] // Months
readTime[6] // Years

```


When setting the alarm, the interrupt bool will dictate if the interrupt pin should follow the alarm flag in the register. 
If set the INT pin will be pulled low when the alarm is activated. This needs to be reset using the resetAlarm() function. 


Added is an example of the code for mbed 6 on a STM32F446RE IC.

