#include "rtc_rv3028.hpp"
#include "g4_iic.h"
#include <stdio.h>

RTC_RV3028::RTC_RV3028(I2C_TypeDef *iic):
    iic_(iic)
{
}


bool RTC_RV3028::readRegister(uint8_t reg_addr, uint8_t *data)
{
    if (1 != iic_transmit(iic_, ADDRESS, &reg_addr, 1))
        return false;
    if (1 != iic_receive(iic_, ADDRESS, data, 1))
        return false;

    return true;
}

bool RTC_RV3028::writeRegister(uint8_t reg_addr, uint8_t data)
{
    uint8_t buf[2] = {reg_addr, data};
    if (2 != iic_transmit(iic_, ADDRESS, buf, 2))
        return false; 
    return true;
}



uint8_t RTC_RV3028::readID()
{
    RegID id;
    if (!readRegister(RegID::ADDR, &(id.raw)))
    {
        return 0xFF; // return invalid ID when read failed
    }
    return id.raw;
}

RTC_RV3028::RTC_Time RTC_RV3028::getCurrentTime()
{
    RegClock reg_clock;
    uint8_t reg_addr = reg_clock.getAddress();
    iic_transmit(iic_, ADDRESS, (uint8_t *)&(reg_addr), 1);
    iic_receive(iic_, ADDRESS, (uint8_t *)&reg_clock, sizeof(RegClock));

    return RegClock2RTCDate(reg_clock);
}

RTC_RV3028::RTC_Time RTC_RV3028::RegClock2RTCDate(RegClock &reg_clock)
{
    RTC_Time time;
    time.second = (reg_clock.seconds & 0x0F) + ((reg_clock.seconds >> 4) * 10);
    time.minute = (reg_clock.minutes & 0x0F) + ((reg_clock.minutes >> 4) * 10);
    time.hour = reg_clock.hours & 0x1F;
    time.weekday = reg_clock.weekday & 0x07;
    time.date = (reg_clock.date & 0x0F) + ((reg_clock.date >> 4) * 10);
    time.month = reg_clock.month & 0x1F;
    time.year = (reg_clock.year & 0x0F) + ((reg_clock.year >> 4) * 10);
    return time;
}

void RTC_RV3028::printCurrentTime()
{
    RTC_Time time = getCurrentTime();
    printf("Current time: %02u:%02u:%02u, %02u/%02u/20%02u, weekday: %u\n\r",
            time.hour, time.minute, time.second, time.date, time.month, time.year, time.weekday);
}
    
void RTC_RV3028::setSecMin(uint8_t minute, uint8_t second)
{
    uint8_t data[3] = {
        RegClock::ADDR,
        dec2Bcd(second),
        dec2Bcd(minute)};
    iic_transmit(iic_, ADDRESS, data, 3);
}

RTC_RV3028::RegStatus RTC_RV3028::getStatus()
{
    RegStatus status;
    if (!readRegister(RegStatus::ADDR, &(status.raw)))
    {
        status.raw = 0xFF; 
    }
    return status;
}


void RTC_RV3028::clearInterruptFlags()
{
    RegStatus status;
    writeRegister(RegStatus::ADDR, status.raw);
}


void RTC_RV3028::disableAllInterrupts()
{
    RegControl2 cr2;
    writeRegister(RegControl2::ADDR, cr2.raw);
}

void RTC_RV3028::enableAlarmInterrupt()
{
    RegControl2 cr2;
    cr2.bits.aie = 1;
    writeRegister(RegControl2::ADDR, cr2.raw);
}

void RTC_RV3028::setAlarmRegisters(uint8_t seconds, uint8_t minutes)
{
    RegMinutesAlarm min_alarm;
    min_alarm.bits.minute = seconds > 0 ? dec2Bcd(minutes + 1) : dec2Bcd(minutes); 
    min_alarm.bits.enable = 0;
    writeRegister(min_alarm.getAddress(), min_alarm.raw);
}

void RTC_RV3028::setAlarm(uint8_t second, uint8_t minute)
{
    clearInterruptFlags();  
    disableAllInterrupts();

    setAlarmRegisters(second, minute);

    uint8_t rtc_sec = second > 0 ?(60 - second) : 0;
    setSecMin(0, rtc_sec);

    enableAlarmInterrupt();
}