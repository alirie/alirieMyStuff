#include <stm32f4xx.h>

#define MOSI_Pin    	GPIO_Pin_1
#define MISO_Pin    	GPIO_Pin_2
#define SCK_Pin        	GPIO_Pin_0
#define CSN_Pin        	GPIO_Pin_1
#define CE_Pin        	GPIO_Pin_0
#define IRQ_Pin        	GPIO_Pin_0       //ich brauche unbedingt einen IRQ Pin...


//here are some register addresses... in hex
//I don't know if I'll need those... o.o
#define REG_CONFIG		0x00
#define REG_EN_AA		0x01
#define REG_EN_RXADDR   0x02
#define REG_SETUP_AW    0x03
#define REG_SETUP_RETR  0x04
#define REG_RF_CH       0x05
#define REG_RF_SETUP    0x06
#define REG_STATUS      0x07
#define REG_OBSERVE_TX  0x08
#define REG_RPD         0x09
#define REG_RX_ADDR_P0  0x0a
#define REG_RX_ADDR_P1  0x0b
#define REG_RX_ADDR_P2  0x0c
#define REG_RX_ADDR_P3  0x0d
#define REG_RX_ADDR_P4  0x0e
#define REG_RX_ADDR_P5  0x0f
#define REG_TX_ADDR     0x10
#define REG_RX_PW_P0    0x11
#define REG_RX_PW_P1    0x12
#define REG_RX_PW_P2    0x13
#define REG_RX_PW_P3    0x14
#define REG_RX_PW_P4    0x15
#define REG_RX_PW_P5    0x16
#define REG_FIFO_STATUS	0x17
#define REG_DYNPD       0x1c //stay away from this one.
#define FEATURE         0x1d //stay away from bit three to seven !!


void init(void);
void read(void);
void write(void);
void printBuffers(void);


unsigned char SPI2_readBuf(unsigned char reg,unsigned char *pBuf, unsigned char bytes);
uint8_t SPI2_readWrite(uint8_t byte);