#ifndef G4_SPI_H_
#define G4_SPI_H_

#include "stm32g474xx.h"
#include "stm32g4xx.h"
#include <cmsis_gcc.h>
#include <stdbool.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif 


/**
 *  @brief Configuretion of the SPIx_CR1 register.
 */
/* BR Baud Rate Prescaler */
#define SPI_baudrate_prescaler_2     (0U << SPI_CR1_BR_Pos)
#define SPI_baudrate_prescaler_4     (1U << SPI_CR1_BR_Pos)
#define SPI_baudrate_prescaler_8     (2U << SPI_CR1_BR_Pos)
#define SPI_baudrate_prescaler_16    (3U << SPI_CR1_BR_Pos)
#define SPI_baudrate_prescaler_32    (4U << SPI_CR1_BR_Pos)
#define SPI_baudrate_prescaler_64    (5U << SPI_CR1_BR_Pos)
#define SPI_baudrate_prescaler_128   (6U << SPI_CR1_BR_Pos)
#define SPI_baudrate_prescaler_256   (7U << SPI_CR1_BR_Pos)
/* CPOL Clock Polarity */
#define SPI_clock_polarity_low       (0U << SPI_CR1_CPOL_Pos)
#define SPI_clock_polarity_high      (1U << SPI_CR1_CPOL_Pos)
/* CPHA Clock Phase */
#define SPI_clock_phase_1edge        (0U << SPI_CR1_CPHA_Pos)
#define SPI_clock_phase_2edge        (1U << SPI_CR1_CPHA_Pos)
/* RXONLY Full/Half Dublex (not configuralbe 0)*/
#define SPI_duplex_full              (0U << SPI_CR1_RXONLY_Pos)
#define SPI_duplex_half              (1U << SPI_CR1_RXONLY_Pos)
/* BIDIMODE (not configuralbe 0) */
#define SPI_bidi_two_lines           (0U << SPI_CR1_BIDIMODE_Pos)
#define SPI_bidi_one_line            (1U << SPI_CR1_BIDIMODE_Pos)
/* BIDIOE  tx/rx enable in case of BIDIMODE (not configuralbe 0)*/ 
#define SPI_tx_only                  (0U << SPI_CR1_BIDIOE_Pos)
#define SPI_rx_only                  (1U << SPI_CR1_BIDIOE_Pos)
/* LSBFIRST */
#define SPI_format_msb_first         (0U << SPI_CR1_LSBFIRST_Pos)
#define SPI_format_lsb_first         (1U << SPI_CR1_LSBFIRST_Pos)
/* CRCEN (not configurable 0) */
#define SPI_crc_disable              (0U << SPI_CR1_CRCEN_Pos)
#define SPI_crc_enable               (1U << SPI_CR1_CRCEN_Pos)
/* CRCNEXT (not configurable 0) */
#define SPI_crc_data_next            (0U << SPI_CR1_CRCNEXT_Pos)
#define SPI_crc_next                 (1U << SPI_CR1_CRCNEXT_Pos)
/* CRCL (not configurable 0)*/
#define SPI_crc_lenght_8bit          (0U << SPI_CR1_CRCL_Pos)
#define SPI_crc_lenght_16bit         (1U << SPI_CR1_CRCL_Pos)
/* SSM Software Slave Management (not configurable 0)*/
#define SPI_ssm_disable              (0U << SPI_CR1_SSM_Pos)
#define SPI_ssm_enable               (1U << SPI_CR1_SSM_Pos)
/* SSI Internal Slave Select (not configuralbe 0) */
#define SPI_ssi_low                  (0U << SPI_CR1_SSI_Pos)
#define SPI_ssi_high                 (1U << SPI_CR1_SSI_Pos)
/* MSTR (not configurable 0) */
#define SPI_mode_slave               (0U << SPI_CR1_MSTR_Pos)
#define SPI_mode_master              (1U << SPI_CR1_MSTR_Pos)
/* SPIxCR1 clear value  */
#define SPI_cr1_clear_mask           ( SPI_CR1_BR_Msk | SPI_CR1_CPOL_Msk | SPI_CR1_CPHA_Msk | \
                                       SPI_CR1_RXONLY_Msk | SPI_CR1_BIDIMODE_Msk | SPI_CR1_BIDIOE_Msk | \
                                       SPI_CR1_LSBFIRST_Msk | SPI_CR1_CRCEN_Msk | SPI_CR1_CRCNEXT_Msk | \
                                       SPI_CR1_CRCL_Msk | SPI_CR1_SSM_Msk | SPI_CR1_SSI_Msk | \
                                       SPI_CR1_MSTR_Msk )

/**
 *  @brief Configuration of the SPIx_CR2 register.
 */
/* DS Data Size */
#define SPI_ds_not_used1            (0U << 8U)
#define SPI_ds_not_used2            (1U << 8U)
#define SPI_ds_not_used3            (2U << 8U)
#define SPI_ds_4bit                 (3U << 8U)
#define SPI_ds_5bit                 (4U << 8U)
#define SPI_ds_6bit                 (5U << 8U)
#define SPI_ds_7bit                 (6U << 8U)
#define SPI_ds_8bit                 (7U << 8U)
#define SPI_ds_9bit                 (8U << 8U)
#define SPI_ds_10bit                (9U << 8U)
#define SPI_ds_11bit                (10U << 8U)
#define SPI_ds_12bit                (11U << 8U)
#define SPI_ds_13bit                (12U << 8U)
#define SPI_ds_14bit                (13U << 8U)
#define SPI_ds_15bit                (14U << 8U)
#define SPI_ds_16bit                (15U << 8U)
/* SSOE  SS output enable (not configurable 0) */
#define SPI_ssoe_disable            (0U << 2U)
#define SPI_ssoe_enable             (1U << 2U)
/* FRF frame format (not configurable 0)*/
#define SPI_frf_motorola            (0U << 4U)
#define SPI_frf_ti                  (1U << 4U)
/* NSSP NSS pulse management (not configurable 0) */
#define SPI_nssp_disable            (0U << 3U)
#define SPI_nssp_enable             (1U << 3U)
/* FRXTH Fifo Reception threshold (not configurable 1)*/
#define SPI_frxth_16bit             (0U << 12U)
#define SPI_frxth_8bit              (1U << 12U)
/* SPIx_CR2 clear value */
#define SPI_cr2_clear_mask          (SPI_CR2_FRXTH_Msk | SPI_CR2_NSSP_Msk | SPI_CR2_FRXTH_Msk | \
                                     SPI_CR2_SSOE_Msk | SPI_CR2_DS_Msk )

/**
 *  @brief SPI init structure
 *  Always configure SPI in full-duplex mode.
 *  Alwas disable crc calculation.
 *  NSS management is not handled by hardware.
 *  Always configure as the master.
 *  Always configure in Motorola frmat.
 */
typedef struct 
{
    uint32_t baudrate_prescaler;        /* SPI_boudrate_prescaler_  Prescaler */
    uint32_t clock_polarity;            /* SPI_clock_polarity_      Clock polarity */
    uint32_t clock_phase;               /* SPI_clock_phase_         Clock phase */
    uint32_t frame_format;              /* SPI_format_              LSB/MSB first */
    uint32_t data_size;                 /* SPI_ds_                  Data size */
} spi_init_t; 



/**
 *  @brief Initialize SPI peripheral. 
 *  @param SPIx SPI peripheral (SPI1, SPI2, SPI3)
 *  @param init_struct Pointer to spi_init_t structure with settings for the SPI peripheral.
 *  Make sure that befor calling this function the SCL MISO and MOSI pins are initialized.
 */
void spi_init(SPI_TypeDef *spix, spi_init_t *init_struct)
{
    MODIFY_REG(spix->CR1, SPI_cr1_clear_mask, 
            init_struct->baudrate_prescaler | init_struct->clock_polarity | 
            init_struct->clock_phase        | init_struct->frame_format   |
            SPI_mode_master                 | SPI_duplex_full             | 
            SPI_bidi_two_lines              | SPI_ssm_enable              | 
            SPI_tx_only                     | SPI_crc_disable             | 
            SPI_crc_data_next               | SPI_ssi_high                | 
            SPI_crc_lenght_8bit);

    MODIFY_REG(spix->CR2, SPI_cr2_clear_mask,
            init_struct->data_size          | SPI_frxth_8bit     |
            SPI_nssp_disable                | SPI_frf_motorola   |
            SPI_ssoe_disable);
}

/**
 *  @brief Enables SPIx peripheral.
 */
__STATIC_INLINE void spi_enable(SPI_TypeDef *spix)
{
    SET_BIT(spix->CR1, SPI_CR1_SPE);
}

/**
 *  @brief Get the current level of the SPIx transmit FIFO. 
 *  @return Returns one of the following values:
 */
#define SPI_tx_fifo_empty        0x0U
#define SPI_tx_fifo_quater_full  (SPI_SR_FTLVL_0)
#define SPI_tx_fifo_half_full    (SPI_SR_FTLVL_1)
#define SPI_tx_fifo_full         (SPI_SR_FTLVL_1 | SPI_SR_FTLVL_0)
__STATIC_INLINE uint32_t spi_get_tx_fifo_level(SPI_TypeDef *spix)
{
    return (READ_BIT(spix->SR, SPI_SR_FTLVL_Msk));
}


/**
 *  @brief Get the current level of the SPIx receive FIFO.
 *  @return Returns one of the following values:
 */
#define SPI_rx_fifo_empty        0x0U
#define SPI_rx_fifo_quater_full  (SPI_SR_FRLVL_0)
#define SPI_rx_fifo_half_full    (SPI_SR_FRLVL_1)
#define SPI_rx_fifo_full         (SPI_SR_FRLVL_1 | SPI_SR_FRLVL_0)
__STATIC_INLINE uint32_t spi_get_rx_fifo_level(SPI_TypeDef *spix)
{
    return (READ_BIT(spix->SR, SPI_SR_FRLVL_Msk));
}

/**
 *  @brief Get status of the SPIx fifo flag.
 *  @return True if fifo is empty, false otherwise.
 */
__STATIC_INLINE bool spi_is_tx_fifo_empty(SPI_TypeDef *spix)
{
    if  (READ_BIT(spix->SR, SPI_SR_TXE))
    {
        return true;
    }
    else
    {
        return false;
    }
}


/**
 *  @brief Writes uint8_t to tx fifo.
 */
__STATIC_INLINE void spi_write_tx_fifo(SPI_TypeDef *spix, uint8_t data)
{
#if defined (__GNUC__)
    __IO uint8_t *spidr = ((__IO uint8_t *)&spix->DR);
    *spidr = data;
#else
    *((__IO uint8_t *)&spix->DR) = data;
#endif /* __GNUC__ */
}

/**
 *  @brief Reads uint8_t from rx fifo.
 */
__STATIC_INLINE uint8_t spi_read_rx_fifo(SPI_TypeDef *spix)
{
    return (*((__IO uint8_t *)&spix->DR));
}

/**
 *  @brief Get the busy flag
 *  @return True if SPI is busy, false otherwise.
 */
__STATIC_INLINE bool spi_is_busy(SPI_TypeDef *spix)
{
    if (READ_BIT(spix->SR, SPI_SR_BSY))
    {
        return true;
    }
    else
    {
        return false;
    }
}


/**
 *  @brief Transit data through SPI peripheral.
 *  Polling function.
 */
void spi_tx_only(SPI_TypeDef *spix, uint8_t *data, uint32_t size)
{

}

void spi_rx_only(SPI_TypeDef *spix, uint8_t *data, uint32_t size)
{

}

void spi_tx_rx(SPI_TypeDef *spix, uint8_t *tx_data, uint8_t *rx_data, size_t size)
{
    /* Flush tx and rx fifo */

    /* Make sure SPIx is enables */  
    spi_enable(spix);

    size_t idx = 0;
    while (idx < size)
    {
        while (!spi_is_tx_fifo_empty(spix));
        spi_write_tx_fifo(spix, tx_data[idx]);
        idx += 1;
    }

    while(spi_is_busy(spix));
}


#ifdef __cplusplus
}
#endif

#endif // G4_SPI_H_