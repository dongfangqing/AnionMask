#ifndef _SPI_DEVICE_H_
#define _SPI_DEVICE_H_


#include "stm32f0xx.h"



//#define SPI1_MASTER_MODE

#define SPI1_SLAVE_MODE







#ifdef	SPI1_MASTER_MODE
/*********************************************************************************************
*****************************************Master Mode******************************************
**********************************************************************************************/

#define SPI_NSS_LOW()       GPIO_ResetBits(GPIOA, GPIO_Pin_10)
#define SPI_NSS_HIGH()      GPIO_SetBits(GPIOA, GPIO_Pin_10)


//#define DEVICE_SPIFLASH
#define DEVICE_OLED



#ifdef DEVICE_SPIFLASH
void SPI1_InitConfig(void);
uint8_t SPI_RW(uint8_t byte);
uint16_t SPI_RWHalfWord(uint16_t HalfWord);

#elif defined(DEVICE_OLED)
void SPI2_InitConfig(void);
void SPI_WriterByte(uint8_t dat);

#endif

#ifdef DEVICE_SPIFLASH
/*********************************************************************************************
**************************************SPI_Flash define****************************************
**********************************************************************************************/

#define  sFLASH_ID              0XEF4017 		//W25Q64


uint8_t SPI_FLASH_ReadByte(void);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr);
uint32_t SPI_FLASH_ReadDeviceID(void);
uint32_t SPI_FLASH_ReadID(void);
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_SectorErase(uint32_t SectorAddr);

#elif defined(DEVICE_OLED)

#endif

#elif defined(SPI1_SLAVE_MODE)
/*********************************************************************************************
*******************************************Slave Mode*****************************************
**********************************************************************************************/

void SPI2_InitConfig(void);
void SPI_NVIC_InitConfig(void);

uint8_t SPI_RW(uint8_t byte);

#endif


void SPI_InitConfig(void);


#endif
