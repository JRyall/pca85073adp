#include "PCA85073ADP.h"
#include <cstdint>
#include <string>


//Converting from binary to decimal
uint8_t bcdToDec(uint8_t bcd) 
{    
    return 10*(bcd/16) + bcd % 16; 
}
//converting from decimal to binary
uint8_t decToBcd(uint8_t dec) 
{
    return 16*(dec/10) + dec % 10; 
}

//init the rtc on the I2C
PCA85073ADP::PCA85073ADP(I2C &i2c) : _i2c(i2c) 
{
    _i2c.frequency(100000);
}

//Set the time
void PCA85073ADP::setTime(uint8_t hour, uint8_t min, uint8_t sec) 
{
    char data[4];
    data[0] = RTC_SECOND_ADDR;                  //select the seconds register
    data[1] = 16*(sec/10) + sec % 10;           //seconds byte
    data[2] = 16*(min/10) + min % 10;           //mins byte
    data[3] = 16*(hour/10) + hour % 10;         //hours byte
    _i2c.write(I2C_ADDR, data, 4);              //write the information to the I2C bus
}

//set the date
void PCA85073ADP::setDate(uint8_t weekday, uint8_t day, uint8_t month, uint8_t yr)
{
    char data[5];
    data[0] = RTC_DAY_ADDR;                     //select the day register
    data[1] = 16*(day/10) + day % 10;           //days byte
    data[2] = 16*(weekday/10) + weekday % 10;   //weekday byte (0=Sun, 1=Mon, 2=Tues...)
    data[3] = 16*(month/10) + month % 10;       //month byte
    data[4] = 16*(yr/10) + yr % 10;             //year byte (0 - 99)
    _i2c.write(I2C_ADDR, data, 5);              //write the information to the I2C bus
}

//read the time
char PCA85073ADP::readTime(int n)
{
    char addr[1];
    addr[0] = RTC_SECOND_ADDR;                  //select the second register
    _i2c.write(I2C_ADDR, addr, 1);              //write the address to read from

    char data[7];
    _i2c.read(I2C_ADDR, data, 7);               //read the output data

    char readTimeOutput[7];
    readTimeOutput[0] = bcdToDec(data[0]); //sec
    readTimeOutput[1] = bcdToDec(data[1]); //min
    readTimeOutput[2] = bcdToDec(data[2]); //hour
    readTimeOutput[3] = bcdToDec(data[3]); //day
    readTimeOutput[4] = bcdToDec(data[4]); //weekday
    readTimeOutput[5] = bcdToDec(data[5]); //month
    readTimeOutput[6] = bcdToDec(data[6]); //year
    return readTimeOutput[n];
}

// called on setAlarm()
void PCA85073ADP::enableAlarm()	
{ 
    char addr[1];
    addr[0] = RTC_CTRL_2;                       //select the control_2 register
    _i2c.write(I2C_ADDR, addr, 1);              //write the address to read from

    char readData[1];                           
    _i2c.read(I2C_ADDR, readData, 1);           //read the output data


    char bit = 0b10000000;                      //bit 7 needs to be 1
    char data[2];
    data[0] = RTC_CTRL_2;                       //select the control_2 address
    data[1] = readData[0] | bit ;               //bitwise the byte read from the C2 register and change bit 7 

    _i2c.write(I2C_ADDR, data, 2);              //write the new byte to the register
}

//set the alarm
void PCA85073ADP::setAlarm(uint8_t alarm_hour, uint8_t alarm_minute, uint8_t alarm_second, uint8_t alarm_day, uint8_t alarm_weekday)
{

    char data[6];

    data[0] = RTC_SECOND_ALARM;                 //select the alarm second register
    data[1] = decToBcd(alarm_second);           
    data[2] = decToBcd(alarm_minute); 
    data[3] = decToBcd(alarm_hour); 
    data[4] = decToBcd(alarm_day);
    data[5] = decToBcd(alarm_weekday);

    _i2c.write(I2C_ADDR, data, 6);              //write alarm bytes

    enableAlarm();                              //call the enable alarm function to enable the INT pin to be set low if alarm is activated
}

//check if the alarm has been activated
bool PCA85073ADP::checkAlarmFlag()
{

    char addr[1];
    addr[0] = RTC_CTRL_2;                       //select the control_2 register
    _i2c.write(I2C_ADDR, addr, 1);

    char data[1];
    _i2c.read(I2C_ADDR, data, 1);               

    int flag;
    flag = data[0]  >> 6;                       //read the control_2 byte and check if bit 6 is 1
    if(flag == 3)                               //set to 3 as reading byte 6 and 7 (byte 7 set to 1 when inturrupt pin is enabled)
    {
        return true;
    }
    return false;
}

//read the alarm time data
char PCA85073ADP::readAlarm(int n)
{

    char addr[1];
    addr[0] = RTC_SECOND_ALARM;                 //select the alarm second register
    _i2c.write(I2C_ADDR, addr, 1);

    char data[7];
    _i2c.read(I2C_ADDR, data, 5);               //read the data from the 5 bytes containing the info

    char output[5];
    output[0] = 10*(data[0]/16) + data[0] % 16; //sec
    output[1] = 10*(data[1]/16) + data[1] % 16; //min
    output[2] = 10*(data[2]/16) + data[2] % 16; //hour
    output[3] = 10*(data[3]/16) + data[3] % 16; //day
    output[4] = 10*(data[4]/16) + data[4] % 16; //weekday


    return output[n];

}

//reset the alarm
void PCA85073ADP::resetAlarm()
{
    
    char addr[1];
    addr[0] = RTC_CTRL_2;                       //select the control_2 register
    _i2c.write(I2C_ADDR, addr, 1);

    char readData[1];
    _i2c.read(I2C_ADDR, readData, 1);           //read the byte from C2

    char bit = 0b00000000;                      //set a 0 bit byte
    char data[2];
    data[0] = RTC_CTRL_2;
    data[1] = readData[0] & bit ;               //bitwise to check and reset the bits
    _i2c.write(I2C_ADDR, data, 2);              //write the data
    
}


uint8_t PCA85073ADP::getSecond()
{
char addr[1];
    addr[0] = RTC_SECOND_ADDR;
    _i2c.write(I2C_ADDR, addr, 1);

    char data[1];
    _i2c.read(I2C_ADDR, data, 1);


    uint8_t sec;
    sec = 10*(data[0]/16) + data[0] % 16; 
    

    return sec;
}


uint8_t PCA85073ADP::getMinute()
{
char addr[1];
    addr[0] = RTC_MINUTE_ADDR;
    _i2c.write(I2C_ADDR, addr, 1);

    char data[1];
    _i2c.read(I2C_ADDR, data, 1);


    uint8_t min;
    min = 10*(data[0]/16) + data[0] % 16; 
    

    return min;
}


uint8_t PCA85073ADP::getHour()
{
char addr[1];
    addr[0] = RTC_HOUR_ADDR;
    _i2c.write(I2C_ADDR, addr, 1);

    char data[1];
    _i2c.read(I2C_ADDR, data, 1);


    uint8_t hr;
    hr = 10*(data[0]/16) + data[0] % 16; 
    

    return hr;
}


uint8_t PCA85073ADP::getDay()
{
char addr[1];
    addr[0] = RTC_DAY_ADDR;
    _i2c.write(I2C_ADDR, addr, 1);

    char data[1];
    _i2c.read(I2C_ADDR, data, 1);


    uint8_t dy;
    dy = 10*(data[0]/16) + data[0] % 16; 
    

    return dy;
}


uint8_t PCA85073ADP::getWeekday()
{
char addr[1];
    addr[0] = RTC_WDAY_ADDR;
    _i2c.write(I2C_ADDR, addr, 1);

    char data[1];
    _i2c.read(I2C_ADDR, data, 1);


    uint8_t wdy;
    wdy = 10*(data[0]/16) + data[0] % 16; 
    

    return wdy;
}


uint8_t PCA85073ADP::getMonth()
{
char addr[1];
    addr[0] = RTC_MONTH_ADDR;
    _i2c.write(I2C_ADDR, addr, 1);

    char data[1];
    _i2c.read(I2C_ADDR, data, 1);


    uint8_t mnth;
    mnth = 10*(data[0]/16) + data[0] % 16; 
    

    return mnth;
}


uint16_t PCA85073ADP::getYear()
{
char addr[1];
    addr[0] = RTC_YEAR_ADDR;
    _i2c.write(I2C_ADDR, addr, 1);

    char data[1];
    _i2c.read(I2C_ADDR, data, 1);


    uint8_t yr;
    yr = (10*(data[0]/16) + data[0] % 16) ; 
    

    return yr;
}


uint8_t PCA85073ADP::getAlarmSecond()
{
char addr[1];
    addr[0] = RTC_SECOND_ALARM;
    _i2c.write(I2C_ADDR, addr, 1);

    char data[1];
    _i2c.read(I2C_ADDR, data, 1);


    uint8_t asec;
    asec = 10*(data[0]/16) + data[0] % 16; 
    

    return asec;
}


uint8_t PCA85073ADP::getAlarmMinute()
{
char addr[1];
    addr[0] = RTC_MINUTE_ALARM;
    _i2c.write(I2C_ADDR, addr, 1);

    char data[1];
    _i2c.read(I2C_ADDR, data, 1);


    uint8_t amin;
    amin = 10*(data[0]/16) + data[0] % 16; 
    

    return amin;
}


uint8_t PCA85073ADP::getAlarmHour()
{
char addr[1];
    addr[0] = RTC_HOUR_ALARM;
    _i2c.write(I2C_ADDR, addr, 1);

    char data[1];
    _i2c.read(I2C_ADDR, data, 1);


    uint8_t ahr;
    ahr = 10*(data[0]/16) + data[0] % 16; 
    

    return ahr;
}


uint8_t PCA85073ADP::getAlarmDay()
{
char addr[1];
    addr[0] = RTC_DAY_ALARM;
    _i2c.write(I2C_ADDR, addr, 1);

    char data[1];
    _i2c.read(I2C_ADDR, data, 1);


    uint8_t ady;
    ady = 10*(data[0]/16) + data[0] % 16; 
    

    return ady;
}


uint8_t PCA85073ADP::getAlarmWeekday()
{
char addr[1];
    addr[0] = RTC_WDAY_ALARM;
    _i2c.write(I2C_ADDR, addr, 1);

    char data[1];
    _i2c.read(I2C_ADDR, data, 1);


    uint8_t awdy;
    awdy = 10*(data[0]/16) + data[0] % 16; 
    

    return awdy;
}