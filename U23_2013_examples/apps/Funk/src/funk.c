
#include "funk.h"
#include <System.h>
#include <stdio.h>
#include <string.h>
#include <stm32f4xx.h>
#include <stm32f4xx_usart.h>

//#include "spi.h"
#include "shiftbrite.h"
#include "bitbanging.h"

uint16_t addressBuffer[8];
uint16_t addressBufferCount = 0;
uint16_t dataBuffer[8];
uint16_t dataBufferCount = 0;

void init(void)
{ 
  for (int i = 0; i < 8; ++i)
  {
    addressBuffer[i] = 0;
    dataBuffer[i] = 5;
  }

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

 //RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

  GPIO_Init(GPIOC, &(GPIO_InitTypeDef){
    .GPIO_Speed = GPIO_Speed_50MHz,
    .GPIO_Mode = GPIO_Mode_AF,
    //.GPIO_OType = GPIO_OType_PP,
    //.GPIO_PuPd = GPIO_PuPd_,
    .GPIO_Pin = SCK_Pin | MISO_Pin | MOSI_Pin
  });

  /*GPIO_Init(GPIOC, &(GPIO_InitTypeDef){
    .GPIO_Speed = GPIO_Speed_50MHz,
    .GPIO_Mode = GPIO_Mode_OUT,
   // .GPIO_OType = GPIO_OType_PP,
    //.GPIO_PuPd = GPIO_PuPd_,
    .GPIO_Pin = MOSI_Pin
  });

  GPIO_Init(GPIOC, &(GPIO_InitTypeDef){
    .GPIO_Speed = GPIO_Speed_50MHz,
    .GPIO_Mode = GPIO_Mode_IN,
   // .GPIO_OType = GPIO_OType_PP,
    //.GPIO_PuPd = GPIO_PuPd_,
    .GPIO_Pin = MISO_Pin //| MOSI_Pin
  });*/

  GPIO_Init(GPIOD, &(GPIO_InitTypeDef){
    .GPIO_Speed = GPIO_Speed_50MHz,
    .GPIO_Mode = GPIO_Mode_OUT,
    //.GPIO_OType = GPIO_OType_PP,
    //.GPIO_PuPd = GPIO_PuPd_UP,
    .GPIO_Pin = CE_Pin | CSN_Pin
  });

  GPIO_Init(GPIOE, &(GPIO_InitTypeDef){
    .GPIO_Speed = GPIO_Speed_50MHz,
    .GPIO_Mode = GPIO_Mode_IN,
    //.GPIO_OType = GPIO_OType_PP,
    //.GPIO_PuPd = GPIO_PuPd_UP,
    .GPIO_Pin = IRQ_Pin
  });


  SPI_Init(SPI2, &(SPI_InitTypeDef){
    .SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4,
    .SPI_CPHA = SPI_CPHA_1Edge,
    .SPI_CPOL = SPI_CPOL_Low,
    .SPI_CRCPolynomial = 7,
    .SPI_DataSize = SPI_DataSize_8b,
    .SPI_Direction = SPI_Direction_2Lines_FullDuplex,
    .SPI_FirstBit = SPI_FirstBit_MSB,
    .SPI_Mode = SPI_Mode_Master,
    .SPI_NSS = SPI_NSS_Soft
  });

  /*EXTI_Init(&(EXTI_InitTypeDef){
    .EXTI_Line = EXTI_Line0,
    .EXTI_Mode = EXTI_Mode_Interrupt,
    .EXTI_Trigger = EXTI_Trigger_Rising,
    .EXTI_LineCmd = ENABLE
   });

  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource0);*/

  NVIC_Init(&(NVIC_InitTypeDef){
    .NVIC_IRQChannel = USART1_IRQChannel,
    .NVIC_IRQChannelPreemptionPriority = 0,
    .NVIC_IRQChannelSubPriority = 0,
    .NVIC_IRQChannelCmd = ENABLE
  });


  GPIO_PinAFConfig(GPIOC, GPIO_PinSource0, GPIO_AF_SPI2);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_SPI2);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF_SPI2);

  SPI_Cmd(SPI2, ENABLE);

  Delay(100);
  //Hopefully in Powerdown Mode... have to set PWR_UP bit in CONFIG Reg 1

  uint8_t buf[5]={0};

  GPIO_SetBits(GPIOD, CE_Pin);

  printf("status: %i\n", SPI2_readBuf(0x10, buf, 5));
  //GPIO_SetBits(GPIOD, CSN_Pin);


  //Ich glaub der Interrupt war ne ganz dumme Idee-.-

  //SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);
  //SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_TXE, ENABLE);




}

void read(void)
{
  GPIO_ResetBits(GPIOD, CSN_Pin);

  for (int i = 0; i < 8; i++)
  { 
    GPIO_WriteBit(GPIOC, MOSI_Pin, SET);
    printf("%i\n", GPIO_ReadInputDataBit(GPIOC, MISO_Pin));
  }
  GPIO_SetBits(GPIOD, CSN_Pin);
}

void write(void)
{
 
}

void printBuffers(void)
{
  for (int i = 0; i < 8; i++)
  {
    printf("dataBuffer: %i\n", dataBuffer[i]);
  }
  printf("\n");
  for (int i = 0; i < 8; i++)
  {
    printf("addressBuffer: %i\n", addressBuffer[i]);
  }
  printf("\n");
}

void SPI2_IRQHandler(void)
{
 // printf("Interrupt\n");
 if(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==SET)
  {
    printf("RXNE\n");
    dataBuffer[dataBufferCount++] = SPI_I2S_ReceiveData(SPI2);


  }

  if(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==SET)
  {
    printf("TXE\n");
    SPI_I2S_SendData(SPI2, addressBuffer[addressBufferCount++]);

   
  }

  if(dataBufferCount >= 7 || addressBufferCount >= 7)
  {
    SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, DISABLE);
    SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_TXE, DISABLE);
    
    GPIO_ResetBits(GPIOD, CSN_Pin);

    dataBufferCount = 0;
    addressBufferCount = 0;
    
    printBuffers();
  }

}


unsigned char SPI2_readBuf(unsigned char reg,unsigned char *pBuf, unsigned char bytes)
{
        unsigned char status,i;
        GPIO_ResetBits(GPIOD, CSN_Pin);
        // Select register to write to and read status byte
        status = SPI2_readWrite(reg);
        for(i=0;i<bytes;i++)
                pBuf[i] = SPI2_readWrite(0);
        GPIO_SetBits(GPIOD, CSN_Pin);
        return(status);
}
uint8_t SPI2_readWrite(uint8_t byte)
{
   /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
   /* Send byte through the SPI1 peripheral */
   SPI_I2S_SendData(SPI2, byte);
   /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
   /* Return the byte read from the SPI bus */
   return SPI_I2S_ReceiveData(SPI2);
}
