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


## kontrola 