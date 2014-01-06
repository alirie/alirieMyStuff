#include <System.h>
#include <stdio.h>
#include <stm32f4xx.h>

#include "stm32f4xx_adc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
 

int adc_convert(){
 ADC_SoftwareStartConv(ADC1);//Start the conversion
 while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//Processing the conversion
 return ADC_GetConversionValue(ADC1); //Return the converted data
}


int main()
{
	// Do some basic initialization tasks
	InitializeSystem();
	EnableDebugOutput(DEBUG_ITM);


	
 	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOCEN,ENABLE);

 	GPIO_Init(GPIOC, &(GPIO_InitTypeDef){
                .GPIO_Pin = GPIO_Pin_0,
                .GPIO_Mode = GPIO_Mode_AN,
                .GPIO_PuPd = GPIO_PuPd_NOPULL
        });

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

 	ADC_Init(ADC1, &(ADC_InitTypeDef){
                .ADC_DataAlign = ADC_DataAlign_Right,
                .ADC_Resolution = ADC_Resolution_12b,
                .ADC_ContinuousConvMode = ENABLE,
                .ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1,
                .ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None,
                .ADC_NbrOfConversion = 1,
                .ADC_ScanConvMode = DISABLE
        });

 	ADC_Cmd(ADC1, ENABLE);
 	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_144Cycles);	


	while(1)
	{
     	iprintf("Schwarze Magie:%i\n",adc_convert());//Read the ADC converted value
    	Delay(10);
	}
}

