#include <System.h>
#include <stm32f4xx.h>
#include <stdio.h>
#include <math.h>

#define SLAVE_ADDRESS 0x1E

void I2C_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction);
void I2C_restart(I2C_TypeDef* I2Cx, uint8_t addr, uint8_t direction);
void I2C_write(I2C_TypeDef* I2Cx, uint8_t data);
uint8_t I2C_read_ack(I2C_TypeDef* I2Cx);
uint8_t I2C_read_nack(I2C_TypeDef* I2Cx);
void I2C_stop(I2C_TypeDef* I2Cx);

int main()
{
    // Do some basic initialization tasks
    InitializeSystem();

    EnableDebugOutput(DEBUG_ITM);


    // Enable clock for the system configuration controller
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    // Enable clock for GPIOA and GPIOC peripheral
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);


    // Set Pin modes for PA8 (SCL)
    GPIO_Init(GPIOA, &(GPIO_InitTypeDef){
                .GPIO_Speed = GPIO_Speed_2MHz,
                .GPIO_Mode = GPIO_Mode_AF,                //Not OUT, but AF = Alternate Function
                .GPIO_OType = GPIO_OType_OD,
                .GPIO_PuPd = GPIO_PuPd_NOPULL,        //External pullups, so none here
                .GPIO_Pin = GPIO_Pin_8
        });

    // Set Pin modes for PC9 (SDA)
    GPIO_Init(GPIOC, &(GPIO_InitTypeDef){
                .GPIO_Speed = GPIO_Speed_2MHz,
                .GPIO_Mode = GPIO_Mode_AF,                //Not OUT, but AF = Alternate Function
                .GPIO_OType = GPIO_OType_OD,
                .GPIO_PuPd = GPIO_PuPd_NOPULL,        //External pullups, so none here
                .GPIO_Pin = GPIO_Pin_9
        });


        // Set Pin modes for PC0 (DRDY)
    GPIO_Init(GPIOB, &(GPIO_InitTypeDef){
                .GPIO_Speed = GPIO_Speed_2MHz,
                .GPIO_Mode = GPIO_Mode_AF,                //Not OUT, but AF = Alternate Function
                .GPIO_OType = GPIO_OType_OD,
                .GPIO_PuPd = GPIO_PuPd_NOPULL,        //External pullups, so none here
                .GPIO_Pin = GPIO_Pin_0
        });



   EXTI_Init(&(EXTI_InitTypeDef){
        .EXTI_Line = EXTI_Line0,
        .EXTI_Mode = EXTI_Mode_Interrupt,
        .EXTI_Trigger = EXTI_Trigger_Rising_Falling,
        .EXTI_LineCmd = ENABLE
         });

    //Now we configured the EXTI-Controller and the GPIO-Controller
    //Wait... If PA0, PB0, PC0, PD0, etc. are all connected to EXTI0, do the all trigger an interrupt?

    //No, we need to select which GPIO line is actively connected to the EXTI
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);

    //Now the EXTI tells the NVIC (Nested Vectored Interrupt Controller) if something happened
    //The interrupt controller is going to ignore that event, we didn't configure it yet
    //Our standard configuration of the NVIC is 4 bit for the preemption priority and 0 bit for the subpriority
    //see the header in libs/libstm32f4xx/src/misc.c for more information
    NVIC_Init(&(NVIC_InitTypeDef){
        .NVIC_IRQChannel = I2C3_EV_IRQn,
        .NVIC_IRQChannelPreemptionPriority = 0x00,  //lowest priority possible
        .NVIC_IRQChannelSubPriority = 0x00,
        .NVIC_IRQChannelCmd = ENABLE
    });


    //Attach alternate pin functions
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_I2C3);        //SCL
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_I2C3);        //SDA
    //GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_I2C3);


    //Enable I2C3 clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);

    //Configure I2C3
    //NOTE: Excerpt of the source code: "To use the I2C at 400 KHz (in fast mode), the PCLK1 frequency (I2C peripheral input clock) must be a multiple of 10 MHz."
    //PCLK1 is 168MHz in our case and is divided by 4 to get PCLK1 which makes it 42 which is not a multiple of 10. We can't use 400kHz here.
    I2C_Init(I2C3, &(I2C_InitTypeDef){
                .I2C_ClockSpeed = 100000,                        //see note above
                .I2C_Mode = I2C_Mode_I2C,                        //we want raw I2C, no SMBUS or other stuff
                .I2C_DutyCycle = I2C_DutyCycle_2,        //only relevant for fast mode
                .I2C_OwnAddress1 = 0xEE,                        //only relevant for slave mode
                .I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit,        //7-bit addresses, only relevant for slave mode
                .I2C_Ack = I2C_Ack_Disable                        //wether or not to acknowledge automatically
        });


    //Enable interrupt
    I2C_ITConfig(I2C3,I2C_IT_EVT, ENABLE); //Event interrupt
    //I2C_ITConfig(I2C3,I2C_IT_ERR, ENABLE); //Error interrupt

    //Turn that thing on...
    I2C_Cmd(I2C3, ENABLE);


    //Set CRA register
    I2C_start(I2C3, SLAVE_ADDRESS<<1, I2C_Direction_Transmitter);
    I2C_write(I2C3, 0x00); // set pointer to CRA
    I2C_write(I2C3, 0x70); // write 0x70 to CRA
    I2C_stop(I2C3);

    //Set CRB register
    I2C_start(I2C3, SLAVE_ADDRESS<<1, I2C_Direction_Transmitter);
    I2C_write(I2C3, 0x01); // set pointer to CRB
    I2C_write(I2C3, 0xA0); // write 0xA0 to CRB
    I2C_stop(I2C3);

    //Set Mode register
    I2C_start(I2C3, SLAVE_ADDRESS<<1, I2C_Direction_Transmitter);
    I2C_write(I2C3, 0x02); // set pointer to Mode
    I2C_write(I2C3, 0x00); // enable continous measurement
    I2C_stop(I2C3);

    //Read Status register, just for the lulz
    uint8_t status = 0;
    I2C_start(I2C3, SLAVE_ADDRESS<<1, I2C_Direction_Transmitter);
    I2C_write(I2C3, 0x09); // set pointer to Status register
    I2C_restart(I2C3, SLAVE_ADDRESS<<1, I2C_Direction_Receiver);        //repeated start, now reading
    status = I2C_read_nack(I2C3);        //read byte, nack and stop

    printf("Status Register: 0x%x\r\n", status);

    while(1)
    {
       
       // Delay(10);
    }
}


void I2C_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction)
{
        // wait until I2C1 is not busy anymore
        while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

        // Send I2C1 START condition
        I2C_GenerateSTART(I2Cx, ENABLE);
        
        // wait for I2C1 EV5 --> Slave has acknowledged start condition
        while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

        // Send slave Address for write
        I2C_Send7bitAddress(I2Cx, address, direction);
        
        /* wait for I2C1 EV6, check if
         * either Slave has acknowledged Master transmitter or
         * Master receiver mode, depending on the transmission
         * direction
         */
        if(direction == I2C_Direction_Transmitter)
                while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

        else if(direction == I2C_Direction_Receiver)
                while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
}

void I2C_restart(I2C_TypeDef* I2Cx, uint8_t addr, uint8_t direction)
{
        // Send I2C1 START condition
        I2C_GenerateSTART(I2Cx, ENABLE);

        // wait for I2C1 EV5 --> Slave has acknowledged start condition
        while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

        // Send slave Address for read or write
        I2C_Send7bitAddress(I2Cx, addr, direction);

        //------------------------------------------------------------------------
        // wait for I2C1 EV6, check if Slave has acknowledged Master transmitter
        // or Master receiver mode, depending on the transmission direction
        //------------------------------------------------------------------------
        if(direction == I2C_Direction_Transmitter)
                while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
        else if(direction == I2C_Direction_Receiver)
                while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
}

void I2C_write(I2C_TypeDef* I2Cx, uint8_t data)
{
        I2C_SendData(I2Cx, data);
        // wait for I2C1 EV8_2 --> byte has been transmitted
        while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

uint8_t I2C_read_ack(I2C_TypeDef* I2Cx)
{
        // enable acknowledge of recieved data
        I2C_AcknowledgeConfig(I2Cx, ENABLE);
        // wait until one byte has been received
        while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
        // read data from I2C data register and return data byte
        uint8_t data = I2C_ReceiveData(I2Cx);
        return data;
}

uint8_t I2C_read_nack(I2C_TypeDef* I2Cx)
{
        // disabe acknowledge of received data
        // nack also generates stop condition after last byte received
        // see reference manual for more info
        I2C_AcknowledgeConfig(I2Cx, DISABLE);
        I2C_GenerateSTOP(I2Cx, ENABLE);
        // wait until one byte has been received
        while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
        // read data from I2C data register and return data byte
        uint8_t data = I2C_ReceiveData(I2Cx);
        return data;
}

void I2C_stop(I2C_TypeDef* I2Cx)
{
        // Send I2C1 STOP Condition
        I2C_GenerateSTOP(I2Cx, ENABLE);
}

void I2C3_EV_IRQHandler()
{

    iprintf("Hey, i2c3 interrupt bla\n");
    I2C_start(I2C3, SLAVE_ADDRESS<<1, I2C_Direction_Transmitter);
   
    I2C_write(I2C3, 0x03); // set pointer to X-Axis register
    I2C_stop(I2C3);
        //do a repeated start, we want to read now
       /* I2C_restart(I2C3, SLAVE_ADDRESS<<1, I2C_Direction_Receiver);
    
        int16_t headings[3];
        //X-Axis
        headings[0] = I2C_read_ack(I2C3) << 8;
        headings[0] |= I2C_read_ack(I2C3);
        //printf("Hey, i2c3 interrupt bla\n");
        //Z-Axis
        headings[1] = I2C_read_ack(I2C3) << 8;
        headings[1] |= I2C_read_ack(I2C3);
        //printf("Hey, i2c3 interrupt bla\n");
        //Y-Axis
        headings[2] = I2C_read_ack(I2C3) << 8;
        headings[2] |= I2C_read_nack(I2C3);
    
        float headingDegrees = atan2((double)headings[2], (double)headings[0])* 180/M_PI + 180;

        printf("Deg: %f, X: %d, Y: %d, Z: %d\r\n", headingDegrees, headings[0], headings[2], headings[1]);*/


}
void I2C3_ER_IRQHandler()
{
    printf("error...\n");
}