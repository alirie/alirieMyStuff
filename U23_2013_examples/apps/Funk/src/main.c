#include <System.h>
#include <stdio.h>
#include <string.h>
#include <stm32f4xx.h>
#include "funk.h"
#include "bitbanging.h"
//#include "spi.h"
#include "shiftbrite.h"




void main(void)
{
    InitializeSystem();
    EnableDebugOutput(DEBUG_ITM);

  
    init();
    
    //read();
    //printBuffers();
    while(1)
    {

    }
}
