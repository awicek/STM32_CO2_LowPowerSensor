## Requirements:
- low-power
- sd card read/write
- C02 sensor 
- temerature 
- battery powered 


## Microcontroler 
- L0 familly [STM32L021K4T6     ](https://cz.mouser.com/ProductDetail/STMicroelectronics/STM32L021K4T6?qs=dTJS0cRn7ohCWAWNqbLYkA%3D%3D&srsltid=AfmBOooC1myd_uRJ6F0qj_L1tkHHFKe1pLfGc2i5RrUbZ-sZrc3xKs-4)
- L5 222nA in StandBy mode with RTC
- L4 245 nA Standby mode with RTC

## C02 sensors  
- [SCD40-D-R1 CO2 4%, IIC, 4.5-5.5VDC, 2.4 to 5.5V](https://cz.farnell.com/sensirion/scd40-d-r1/gas-detection-sensor-co2-40000ppm/dp/3677905?gross_price=true&CMP=KNC-GCZ-GEN-SHOPPING-PMax-Catch-All-8th-May&mckv=_dc|pcrid||plid||kword||match||slid||product|3677905|pgrid||ptaid||&gad_source=1&gad_campaignid=17420022133&gclid=EAIaIQobChMI-sHUy6yOjgMVo5eDBx0wvQleEAQYFiABEgITF_D_BwE)
- [MH-Z16 NDIR, 5% CO2, UART/PWM, 4.5-5.5VDC](https://www.laskakit.cz/en/mh-z16-ndir-senzor-co2-50000ppm/?utm_source=chatgpt.com)

## Power supply 
- some batteries that have 3.3 volts without the regulator 
- another boost for sensors that requires pover supply 5V 
 


## MT3608

## Used parts                               # kontrola footprints
- MCU STM32L021K4T6
- RTCM RV-3028-C8-32.768kHz-1PPM-TA-QC 
- Switch TPS22917DBVR                       [x]
- LDO TPS7A0228PDBV
- micro-usb connector 10118193-0001LF
- CO2 sensor SCD41-D-R2
- micro-sd socket DM3D-SF(41)
- 1k R CRCW04021K00JNEDC 
- 0.5k R CRCW0603511RFKEAHP 
- 47k R RC0201FR-0747KL 
- 0.1 uF GCM033C71A104KE02J 
- 1 uF TMK105BJ105MV-F
- led 


## co jsem dojebal
- male rezistory a kondiky


# hardware kontrola schematic
- IIC
    - speed = 100 | 400 (CO2 100-400, RTC. 100-400kHz), 10kOhm pull-ups
  


# Hardware to do list 
 1) microsd card connection vith spi
 2) how to desing the iic bus
 3) how to desing the power supply the low power ldo
 4) how to add external crystal for the RTC on the L0 
 5) how to choose parts that can be easily soldered by hand 

# Software to do list 
 1) how to read and write to the sd card 


# ToDo list for the project 
  - [ ] check the input LDO
  - [ ] check the CO2 sensor 
  - [ ] check the RTC 
  - [ ] check the crystal for the MCU
  - [ ] check LDOs for powering up the MCU
  - [ ] check the miscro-sd card socket



# How does the co2 sensor work 

## Good to know
- **power-up time**  max 30 ms. Sensor is ready to accept commands after this time.
- **Soft reset time** max 30 ms
- **SCL ckock frequency** 0 - 400 kHz



## I2C communication
- **I2C address** 0x62 (7-bit)
- **I2C speed** max 400 kHz



# TODO what to buy
  - [x] sd card reader 
  - [x] calfuna 
  - [ ] sd card  that 100% can be used with the spi
  - [x] co2 sensor on the breakout board 
  - [x] soldering paste
  - [x] hotplate for soldering
  - [ ] larger resistors 


# Parts to  order
  - [o] usb-c connector 
  - [o] esd proteciton 
  - [x] low poer ldo 
  - [o] baterry protection
  - [o] battery holder 
  - [o] high power ldo
  - [o] crystal
  - [o] crystal condensators
  - [o] buttons
  - [o] mcu
  - [ ] led
  - [o] jst connectors 1x4
  - [o] jst connectors 1x6
  - [x] co2 sensors
  - [o] sdcard slots
  - [o] rtc
  - [o] high switch low
  - [o] pinheader 

## resistors
  - [o] 4.7 k
  - [o] 20 k
  - [o] 30 k
  - [o] 10 k
  - [o] 47 k
  - [o] 470 

## capacitors
  - [o] 1u 
  - [o] 2.2u
  - [o] 100n 
  - [o] 22 u 
  - [o] 10 u 
  - [o] 200 p