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
    iic_transmit(iic_, ADDRESS, (uint8_t *)&GET_SERIAL_NUMBER, 2);

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
    iic_transmit(iic_, ADDRESS, (uint8_t *)&GET_SENSOR_VARIANT, 2);
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
    iic_transmit(iic_, ADDRESS, (uint8_t *)&START_PERIODIC_MEASUREMENT, 2);
}

SCD4X::Measurement SCD4X::getMeasurement()
{   
    iic_transmit(iic_, ADDRESS, (uint8_t *)&READ_MEASUREMENT, 2);
    sys_delay(2);
    uint8_t data[9];
    iic_receive(iic_, ADDRESS, data, 9);
    Measurement measurement;
    measurement.co2_ppm = (float)(((uint16_t)data[0] << 8)
                        | (uint16_t)data[1]);
    measurement.temperature = -45.0f + 175.0f * (float)(((uint16_t)data[3] << 8)
                            | (uint16_t)data[4]) / 65535.0f;
    measurement.humidity = 100.0f * (float)(((uint16_t)data[6] << 8)
                        | (uint16_t)data[7]) / 65535.0f;
    return measurement;
}