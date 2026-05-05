#ifndef SCD4X_HPP_
#define SCD4X_HPP_

#include "g4_iic.h"



/**
 *  @brief Implementation of High level driver for SCD4X sensor.
 */
class SCD4X
{
public:
    enum SensorVariant
    {   
        SCD40 = 0U,
        SCD41 = 1U,
        ERROR = 2U,
    };
 
    struct Measurement
    {
        float co2_ppm;          // co2 parts per million
        float temperature;      // temperature in Celsius 
        float humidity;         // relative humidity in percent
    };

    SCD4X(I2C_TypeDef *iic);

    uint64_t getSensorSerialNumber();

    SensorVariant getSensorVariant();

    void startPeriodicMeasurement();

    Measurement getMeasurement();
private:    
    I2C_TypeDef *iic_;
    constexpr static uint8_t ADDRESS = 0x62;

    /* All possible registers for SCD4X sensor */
    // ===== Basic commands =====
    constexpr static uint16_t START_PERIODIC_MEASUREMENT      = 0xB121;
    constexpr static uint16_t READ_MEASUREMENT                = 0x05EC;
    constexpr static uint16_t STOP_PERIODIC_MEASUREMENT       = 0x863F;
    // ===== Temperature / altitude =====
    constexpr static uint16_t SET_TEMPERATURE_OFFSET          = 0x1D24;
    constexpr static uint16_t GET_TEMPERATURE_OFFSET          = 0x1823;
    constexpr static uint16_t SET_SENSOR_ALTITUDE             = 0x2724;
    constexpr static uint16_t GET_SENSOR_ALTITUDE             = 0x2223;
    // ===== Pressure =====
    constexpr static uint16_t SET_AMBIENT_PRESSURE            = 0x00E0;
    constexpr static uint16_t GET_AMBIENT_PRESSURE            = 0x00E0;
    // ===== Calibration =====
    constexpr static uint16_t PERFORM_FORCED_RECALIBRATION    = 0x2F36;
    constexpr static uint16_t SET_ASC_ENABLED                 = 0x1624;
    constexpr static uint16_t GET_ASC_ENABLED                 = 0x1323;
    // ===== Low power =====
    constexpr static uint16_t START_LOW_POWER_PERIODIC_MEAS   = 0xAC21;
    constexpr static uint16_t GET_DATA_READY_STATUS           = 0xB8E4;
    // ===== Advanced =====
    constexpr static uint16_t PERSIST_SETTINGS                = 0x1536;
    constexpr static uint16_t GET_SERIAL_NUMBER               = 0x8236;
    constexpr static uint16_t PERFORM_SELF_TEST               = 0x3936;
    constexpr static uint16_t FACTORY_RESET                   = 0x3236;
    constexpr static uint16_t REINIT                          = 0x4636;
    constexpr static uint16_t GET_SENSOR_VARIANT              = 0x2F20;
    // ===== Single shot (SCD41) =====
    constexpr static uint16_t MEASURE_SINGLE_SHOT             = 0x9D21;
    constexpr static uint16_t MEASURE_SINGLE_SHOT_RHT_ONLY    = 0x9621;
    constexpr static uint16_t POWER_DOWN                      = 0xE036;
    constexpr static uint16_t SET_ASC_INITIAL_PERIOD          = 0x4524;
    constexpr static uint16_t GET_ASC_INITIAL_PERIOD          = 0x4023;
    constexpr static uint16_t SET_ASC_STANDARD_PERIOD         = 0x4E24;
    constexpr static uint16_t GET_ASC_STANDARD_PERIOD         = 0x4B23;
};


#endif // SCD4X_HPP_
