#include "scd4x.hpp"

#include "g4_iic.h"
#include "sys_tick.h"

SCD4X::SCD4X(I2C_TypeDef *iic) :
    iic_(iic)
{
}


uint64_t SCD4X::getSensorSerialNumber()
{
    uint8_t data[9]; 
    sendCommand(GET_SERIAL_NUMBER);
    sys_delay(2);

    iic_receive(iic_, ADDRESS, data, 9);
    
    return ((uint64_t)data[0] << 56) | ((uint64_t)data[1] << 48) |
           ((uint64_t)data[2] << 40) | ((uint64_t)data[3] << 32) |
           ((uint64_t)data[4] << 24) | ((uint64_t)data[5] << 16) |
           ((uint64_t)data[6] << 8) | (uint64_t)data[7];
}


SCD4X::SensorVariant SCD4X::getSensorVariant()
{
    uint8_t data[3];
    sendCommand(GET_SENSOR_VARIANT);
    sys_delay(2);
    iic_receive(iic_, ADDRESS, data, 3);

    if (data[0] >> 4 == 0b0001)
    {
        return SCD4X::SensorVariant::SCD41;
    }
    else if (data[0] >> 4 == 0b0000)
    {
        return SCD4X::SensorVariant::SCD40;
    }
    return SCD4X::SensorVariant::SCD40;
}


void SCD4X::startPeriodicMeasurement()
{
    sendCommand(START_PERIODIC_MEASUREMENT);
}


void SCD4X::stopPeriodicMesurement()
{
    sendCommand(STOP_PERIODIC_MEASUREMENT);
}


SCD4X::Measurement SCD4X::getMeasurement()
{   
    sendCommand(READ_MEASUREMENT);
    sys_delay(2);

    uint8_t data[9];
    iic_receive(iic_, ADDRESS, data, 9);

    Measurement measurement = rawData2Mesurement(data);
    return measurement;
}

 
SCD4X::Measurement SCD4X::rawData2Mesurement(uint8_t *data)
{
    Measurement meas;
    
    meas.co2_ppm = (float)(((uint16_t)data[0] << 8) | (uint16_t)data[1]);
    meas.temperature = -45.0f + 175.0f / 65535.0f *
                       (float)(((uint16_t)data[3] << 8) | (uint16_t)data[4]);
    meas.humidity = 100.0f / 65535.f *
                    (float)(((uint16_t)data[6] << 8) | (uint16_t)data[7]);

    return meas;
}

void SCD4X::sendCommand(uint16_t command)
{
    for (size_t i = 0; i < 5; ++i)
    {
        if (2 == iic_transmit(iic_, ADDRESS, (uint8_t *)&command, 2))
        {
            return;
        }
        sys_delay(10);
    }
}


bool SCD4X::getDataReadyStatus()
{ 
    sendCommand(GET_DATA_READY_STATUS);
    sys_delay(2);
    uint8_t data[3];
    iic_receive(iic_, ADDRESS, data, 3);

    if  (((data[0] & 0x07) == 0) && (data[1] == 0))
    {
        return false;
    }
    return true;
}


void SCD4X::singleShotMesurement()
{
    sendCommand(MEASURE_SINGLE_SHOT);
}
