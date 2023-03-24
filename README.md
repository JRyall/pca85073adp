# pca85073adp
Library for use with mbed OS and the pca85073adp RTC
Written by James Ryall 24/03/2023.

Adding this library allows you to read and write to the PCA85073ADP RTC over i2c in Mbed OS. 

Within this library are the following defines:



I2C i2c(PB_7, PB_6)                                       define the I2C Pins
PCA85073ADP rtc(i2c)                                      initialize RTC object

setTime(Hour, Minute, Second)                             Set the time 
setDate(Weekday, Day, Month, Year)                        Set the date
setAlarm(Hour, Minute, Second, Day, Weekday, interrupt)   Set the alarm
checkAlarmFlag()                                          Check the alarm flag
resetAlarm()                                              clears the flags for the inturrupt and the alarm triggered flag

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

When setting the alarm, the interrupt bool will dictate if the interrupt pin should follow the alarm flag in the register. 
If set the pin will be pulled low when the alarm is activated. 

Added is an example of the code for mbed 6 on a STM32F446RE IC.

