#include "PCA85073ADP.h"
#include <cstdint>


// INIT
PCA85073ADP::PCA85073ADP(I2C &i2c) : _i2c(i2c) {
    _i2c.frequency(100000);

}


void PCA85073ADP::setTime(uint8_t hour, uint8_t min, uint8_t sec) {
    
    // Convert values to BCD format and write the time
    _i2c.lock();
    _i2c.start();
    _i2c.write(I2C_ADDR << 1); // to indicate an i2c write, shift the 7 bit address up 1 bit and keep bit 0 as a 0

char seconds[7];
    seconds[0] =  0x00;
    seconds[1] =  decToBcd(sec);

    _i2c.write(RTC_SECOND_ADDR, seconds, 7, false);
    //_i2c.write(RTC_MINUTE_ADDR, decToBcd(min), 7);
    //_i2c.write(RTC_HOUR_ADDR, decToBcd(hour), 5);
    _i2c.stop();
    _i2c.unlock();
}
 
int PCA85073ADP::readTime()
{
// Convert values to BCD format and write the time
    _i2c.lock();
    _i2c.start();
    _i2c.write(I2C_ADDR << 1); // to indicate an i2c write, shift the 7 bit address up 1 bit and keep bit 0 as a 0

    char read[1];
    read[0] =  0x01;
    char data[7];

    _i2c.write(RTC_SECOND_ADDR, read, 1, false);
    _i2c.read(RTC_SECOND_ADDR, data, 7);
    //_i2c.write(RTC_MINUTE_ADDR, decToBcd(min), 7);
    //_i2c.write(RTC_HOUR_ADDR, decToBcd(hour), 5);
    _i2c.stop();
    _i2c.unlock();
    int secs = bcdToDec(data[1]);
    return secs;
}




uint8_t bcdToDec(uint8_t bcd) 
{    
    return 10*(bcd/16) + bcd % 16; 
}
uint8_t decToBcd(uint8_t dec) 
{
    return 16*(dec/10) + dec % 10; 
}