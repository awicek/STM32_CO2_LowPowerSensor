#ifndef RTC_RV3028_HPP_ 
#define RTC_RV3028_HPP_ 

#include "g4_iic.h"
#include <stm32g474xx.h>

/**
 *  @brief Implementation of High level driver for RV-3028-C7 RTC.
 *  The RV-3028-C7 is a low-power real-time clock with IIC interface.
 */
class RTC_RV3028
{
public:
    /**
    *  @brief Struct representing the date 
    */
    struct RTC_Time
    {
        uint8_t second;
        uint8_t minute;
        uint8_t hour;
        uint8_t weekday;
        uint8_t date;
        uint8_t month;
        uint8_t year;
    };


    /**
    *  @brief Constructor.
    *  @param iic IIC peripheral to use for communication with the sensor.
    *  The IIC peripheral mus be pre-initialized.
    */       
    RTC_RV3028(I2C_TypeDef *iic);

    /**
    *  @brief Read device ID.
    *  @return device id, see @ref RegID for details.
    */
    uint8_t readID();

    /**
    *  @brief Get current date and time from the RTC. 
    */
    RTC_Time getCurrentTime();
    
    /**
    *  @brief Set seconds and minutes to the RTC. 
    *  @param minute Minute to set.
    *  @param second Second to set.
    */
    void setSecMin(uint8_t minute, uint8_t second);

    /**
    *  @brief Set Alarm. 
    *  @param seccond Allamr timing seconds.
    *  @param minute Allamr timing minutes.
    */
    void setAlarm(uint8_t second, uint8_t minute);

    /**
    *  @brief Prints current time.
    */
    void printCurrentTime();

private:
    I2C_TypeDef *iic_; 

    /* Registers of the RV-3028-C7 */
    /* IIC address of the RV-3028-C7 */
    constexpr static uint8_t ADDRESS = 0x52;
#pragma pack(push, 1)
    union RegStatus
    {
        uint8_t raw;
        struct
        {
            uint8_t porf    : 1;  /* Power-On Reset Flag. */  
            uint8_t evf     : 1;  /* External Event Flag. */
            uint8_t af      : 1;  /* Alarm Interrupt Flag */
            uint8_t tf      : 1;  /* Periodic Countdown Timer Interrupt Flag */
            uint8_t uf      : 1;  /* Periodic Time Update Interrupt Flag */
            uint8_t bsf     : 1;  /* Backup Switch Flag */
            uint8_t clkf    : 1;  /* Clock Output Interrupt Flag */
            uint8_t eebusy  : 1;  /* EEPROM Memory Busy Flag. 
                                     0: tranfer finished, 1: eeprom read/write in progress */
        } bits;
        constexpr RegStatus(uint8_t value = 0)
            : raw(value)
        {
        }

        constexpr static uint8_t ADDR = 0x0E;
        constexpr uint8_t getAddress() const 
        {
            return ADDR;
        }
    };
    union RegControl1
    {
        uint8_t raw;
        struct
        {
            uint8_t td    : 2;  /* Timer clock freq. selection for PERIODIC COUNTDOWN TIMER. 
                                   0: 4096 Hz, 1: 64 Hz, 2: 1 Hz, 3: 1/60 Hz */
            uint8_t te    : 1;  /* PERIODIC COUNTDOWN TIMER Enable bit. 0: disable, 1: enable */ 
            uint8_t eerd  : 1;  /* EEPROM Memory Refresh Disable bit.   0: enable, 1: disable */ 
            uint8_t usel  : 1;  /* Update Interrupt Select bit.  PERIODIC TIME UPDATE INTERRUPT
                                   0: second update, 1: minute update */ 
            uint8_t wada  : 1;  /* Weekday/Date Alarm selection bit. ALARM INTERRUPT 
                                   0: weekday is used, 1: date is used */ 
            uint8_t resv6 : 1;  /* reserved */  
            uint8_t trpt  : 1;  /* Timer Repeat bit. PERIODIC COUNTDOWN TIMER
                                   0: single mode, 1: repeat mode */
        } bits;
        constexpr RegControl1(uint8_t value = 0)
            : raw(value)
        {
        }

        constexpr static uint8_t ADDR = 0x0F;
        constexpr uint8_t getAddress() const 
        {
            return ADDR;
        }
    };
    union RegControl2 
    {
        uint8_t raw;
        struct
        {
            uint8_t reset : 1;  /* Reset bit. Used for some adjustement. */  
            uint8_t hour  : 1;  /* 12 or 24 hour mode. 0: 24 (def), 1: 12 */  
            uint8_t eie   : 1;  /* Event Interrupt Enable bit */  
            uint8_t aie   : 1;  /* Alarm Interrupt Enable bit */  
            uint8_t tie   : 1;  /* Periodic Countdown Timer Interrupt Enable bit */  
            uint8_t uie   : 1;  /* Periodic Time Update Interrupt Enable bit */  
            uint8_t clkie : 1;  /* Interrupt Controlled Clock Output Enable bit */  
            uint8_t tse   : 1;  /* Time Stamp Enable */  
        } bits;
        constexpr RegControl2(uint8_t value = 0)
            : raw(value)
        {
        }

        constexpr static uint8_t ADDR = 0x10;
        constexpr uint8_t getAddress() const 
        {
            return ADDR;
        }
    };
    union RegID
    {
        uint8_t raw;
        struct
        {
            uint8_t vid : 4;  /* Version ID */  
            uint8_t hid : 4;  /* Hardware ID */  
        } bits;
        constexpr RegID(uint8_t value = 0)
            : raw(value)
        {
        }

        constexpr static uint8_t ADDR = 0x28;
        constexpr uint8_t getAddress() const 
        {
            return ADDR;
        }
    };
    union RegMinutesAlarm
    {
        uint8_t raw;
        struct
        {
            uint8_t minute : 7;  /* Minute value for Alarm in BCD format*/
            uint8_t enable : 1;  /* Alarm enable bit. 0: enable, 1: disable*/  
        } bits;
        constexpr RegMinutesAlarm(uint8_t value = 0x80)
            : raw(value)
        {
        }

        constexpr static uint8_t ADDR = 0x07;
        constexpr uint8_t getAddress() const 
        {
            return ADDR;
        }
    };
    struct RegClock
    {
        uint8_t seconds = 0;
        uint8_t minutes = 0;
        uint8_t hours = 0;
        uint8_t weekday = 0;
        uint8_t date = 0;
        uint8_t month = 0;
        uint8_t year = 0;
        constexpr static uint8_t ADDR = 0x00;
        constexpr uint8_t getAddress() const
        {
            return ADDR;
        }
    };
    
#pragma pack(pop)

    /**
    *  @brief Read Register.
    *  @param reg_addr Address of the register to read.
    *  @param data IN/OUT: pointer to the variable to store the register value.
    *  @return true when successfully read the register, false otherwise.
    */
    bool readRegister(uint8_t reg_addr, uint8_t *data);

    /**
    *  @brief Write Register.
    *  @param reg_addr Address of the register to write.
    *  @param data Value to write to the register.
    *  @return true when successfully write the register, false otherwise.
    */
    bool writeRegister(uint8_t reg_addr, uint8_t data);
   

    RTC_Time RegClock2RTCDate(RegClock &reg_clock);

    RegStatus getStatus();

    void clearInterruptFlags();

    void disableAllInterrupts();

    void enableAlarmInterrupt();

    void setAlarmRegisters(uint8_t second, uint8_t minute);


    inline uint8_t bcd2Dec(uint8_t bcd_val)
    {
        return (bcd_val / 16 * 10) + (bcd_val % 16);
    }

    inline uint8_t dec2Bcd(uint8_t val)
    {
        return (val / 10 * 16) + (val % 10);
    }
};


#endif // RTC_RV3028_HPP_