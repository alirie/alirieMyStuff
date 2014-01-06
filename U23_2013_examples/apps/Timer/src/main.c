#include <System.h>
#include <stdio.h>
#include <stm32f4xx.h>


int globale_Variable = 0;

int main()
{
	// Do some basic initialization tasks
	InitializeSystem();
	EnableDebugOutput(DEBUG_ITM);



	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// Set Pin mode for PA0 (User-Button)
	GPIO_Init(GPIOA, &(GPIO_InitTypeDef){
		.GPIO_Speed = GPIO_Speed_100MHz,
		.GPIO_Mode = GPIO_Mode_IN,
		.GPIO_OType = GPIO_OType_PP,
		.GPIO_PuPd = GPIO_PuPd_NOPULL,	//no internal pullup or pulldown, is present on PCB
		.GPIO_Pin = GPIO_Pin_0
	});

	EXTI_Init(&(EXTI_InitTypeDef){
		.EXTI_Line = EXTI_Line0,
		.EXTI_Mode = EXTI_Mode_Interrupt,
		.EXTI_Trigger = EXTI_Trigger_Rising,
		.EXTI_LineCmd = ENABLE
	});

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	NVIC_Init(&(NVIC_InitTypeDef){
		.NVIC_IRQChannel = EXTI0_IRQn,
		.NVIC_IRQChannelPreemptionPriority = 0x00,	//lowest priority possible
		.NVIC_IRQChannelSubPriority = 0x00,
		.NVIC_IRQChannelCmd = ENABLE
	});


	// Enable clock for GPIOD peripheral

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	// Set Pin modes for PD12 to PD15 (LEDs)
	GPIO_Init(GPIOD, &(GPIO_InitTypeDef){
		.GPIO_Speed = GPIO_Speed_100MHz,
		.GPIO_Mode = GPIO_Mode_AF,		//Not OUT, but AF = Alternate Function
		.GPIO_OType = GPIO_OType_PP,
		.GPIO_PuPd = GPIO_PuPd_UP,
		.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15 //| GPIO_Pin_12 | GPIO_Pin_14
	});

	
	//Attach pin 12 to 15 of GPIOD to alternate pin function on TIM4
	
	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);



	//Enable Timer 4 clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	//We want the timer to tick with a frequency of 1MHz, calculate a prescaler
	uint32_t PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 1000000) - 1;

	//20 kHz PWM period (i.e. 50uS period)
	uint32_t period = 1000000 / 20000;

	//Configure the timer
	TIM_TimeBaseInit(TIM4, &(TIM_TimeBaseInitTypeDef){
		.TIM_Period = period - 1,
		.TIM_Prescaler = PrescalerValue,
		.TIM_ClockDivision = 0,
		.TIM_CounterMode = TIM_CounterMode_Up,
	});
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;





	// PWM1 Mode configuration: Channel1 (GPIOD Pin 12)
	

	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);


	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	//Enable the timer
	TIM_Cmd(TIM4, ENABLE);

	//low duty cycles are not visible, so we set a maximum brigthness here
	uint32_t max_brightness = period / 8;

	//Set some start values
	uint32_t brightLed1 = 1;						//0% brightness
	uint32_t brightLed2 = max_brightness / 2;		//33% brightness
	uint32_t brightLed3 = (max_brightness / 3) * 2;	//66% brightness
	uint32_t brightLed4 = max_brightness / 2;//max_brightness - 1;		//100% brightness

	int direction1 = 1;
	int direction2 = 1;
	int direction3 = 1;
	int direction4 = 1;
	//Set some initial compare values
	
	TIM_SetCompare2(TIM4, brightLed2);

	TIM_SetCompare4(TIM4, brightLed4);

	while (1)
	{
		Delay(4);

		//Make the counter count up and down alternately
		
		if(direction2 == 1){
			brightLed2--; brightLed4++;
		}
		else{
			brightLed2++; brightLed4--;
		}
		//change the direction if we hit the upper or lower bound
		

		if(brightLed2 == max_brightness || brightLed2 == 0)
			direction2 *= -1;


		//Update compare values

		if(globale_Variable == 0)
		{
			TIM_SetCompare2(TIM4, brightLed2);
			TIM_SetCompare4(TIM4, brightLed4);
		}
		else
		{
			TIM_SetCompare1(TIM4, brightLed2);
			TIM_SetCompare3(TIM4, brightLed4);
		}
		
		
		
	}
}EXTI_Line0


//The interrupt handler called automatically
void EXTI0_IRQHandler()
{ 
	//iprintf("%i\n",EXTI_GetITStatus(EXTI_Line0));
	//huh? You talking to me?
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{	

		uint16_t pinX;
		uint16_t pinY;
		uint8_t pinSourceX;
		uint8_t pinSourceY;
		if(globale_Variable==0)
		{
			globale_Variable=1;

			pinX = GPIO_Pin_12;
			pinY = GPIO_Pin_14;
			pinSourceX = GPIO_PinSource12;				
			pinSourceY = GPIO_PinSource14;
		}
		else
		{
			globale_Variable=0;
			pinX = GPIO_Pin_13;
			pinY = GPIO_Pin_15;
			pinSourceX = GPIO_PinSource13;
			pinSourceY = GPIO_PinSource15;
		}

		TIM_Cmd(TIM4, DISABLE);
		TIM_OCInitTypeDef TIM_OCInitStructure;
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		GPIO_DeInit(GPIOD);
		GPIO_Init(GPIOD, &(GPIO_InitTypeDef){
			.GPIO_Speed = GPIO_Speed_100MHz,
			.GPIO_Mode = GPIO_Mode_AF,		//Not OUT, but AF = Alternate Function
			.GPIO_OType = GPIO_OType_PP,
			.GPIO_PuPd = GPIO_PuPd_UP,
			.GPIO_Pin = pinX | pinY
		});
		GPIO_PinAFConfig(GPIOD, pinSourceX, GPIO_AF_TIM4);
		GPIO_PinAFConfig(GPIOD, pinSourceY, GPIO_AF_TIM4);


		
		TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Disable);
		TIM_OC1Init(TIM4, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);
		
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Disable);
		TIM_OC3Init(TIM4, &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Disable);

		TIM_SetCompare1(TIM4, 25);
	
		TIM_SetCompare3(TIM4, 25);

		TIM_Cmd(TIM4, ENABLE);


		//Clear the interrupt bit and tell the controller we handlet the interrupt
		EXTI_ClearITPendingBit(EXTI_Line0);
		//iprintf("HI\n");
	
	}
}