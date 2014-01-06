#include <System.h>
#include <stdio.h>
#include <Synth.h>
#include <Accelerometer.h>
SynthSong song = {
  AudioFreq_22k,
  1,
  &(SynthChannel) {
    SynthRect,
    2,
    (SynthNote[]) {
      { Note_a1, 250, 0xff },
      { Note_Pause, 62, 0x00 },
      /*{ Note_a1, 250, 0xff },
      { Note_Pause, 62, 0x00 },
      { Note_c2, 250, 0xff },
      { Note_Pause, 62, 0x00 },
      { Note_a1, 250, 0xff },
      { Note_Pause, 62, 0x00 },
      { Note_a2, 250, 0xff },
      { Note_Pause, 62, 0x00 },
      { Note_e2, 250, 0xff },
      { Note_Pause, 62, 0x00 },
      { Note_d2, 250, 0xff },
      { Note_Pause, 62, 0x00 },
      { Note_c2, 180, 0xff },
      { Note_Pause, 40, 0x00 },
      { Note_d2, 180, 0xff },
      { Note_Pause, 40, 0x00 },
      { Note_e2, 180, 0xff },
      { Note_Pause, 40, 0x00 },
      { Note_a1, 250, 0xff },
      { Note_Pause, 62, 0x00 },
      { Note_a1, 250, 0xff },
      { Note_Pause, 62, 0x00 },
      { Note_c2, 250, 0xff },
      { Note_Pause, 62, 0x00 },
      { Note_a1, 250, 0xff },
      { Note_Pause, 62, 0x00 },
      { Note_h1, 250, 0xff },
      { Note_Pause, 62, 0x00 },
      { Note_h1, 250, 0xff },
      { Note_Pause, 62, 0x00 },
      { Note_a1, 250, 0xff },
      { Note_Pause, 62, 0x00 },
      { Note_c2, 250, 0xff },
      { Note_Pause, 62, 0x00 },
      { Note_g1, 250, 0xff },
      { Note_Pause, 62, 0x00 },*/
    }
  }
};

int volume_left = 50;
int volume_right = 50;

static void AudioCallback(void *context, int16_t buffer[256])
{
  //iprintf("Called AudioCallback()\n");
  for(int i = 0; i < 128; i++) {
    int16_t x = SynthGetSample(&song);
    buffer[2*i+0] = ((volume_left+18)/100.0)*x;
    buffer[2*i+1] = ((volume_right+18)/100.0)*x;
  }
}



int main(void)
{

  InitializeSystem();

  EnableDebugOutput(DEBUG_ITM);

  // Initialize pins for Accelerometer
  InitializeAccelerometer();

  // Calibrate the Accelerometer on the Start
  CalibrateAccelerometer();

  InitializeLEDs();

  SetLEDs(1);



  InitializeAudio(AudioFreq_22k);

  //iprintf("%i\n", IsAudioInitialized());

  //AudioOn();
 
  
      //iprintf("%i\n", 123);
  PlayAudioWithCallback(AudioCallback, NULL);
  
  SetAudioVolume(96);
  //max: 160 min: 64
  //printf("%i\n", volume);
  while(1)
  {
    printf("left:%3i right:%3i\n", volume_left, volume_right);
    Delay(10);
    int led = 0;
    int8_t Data[3] = {};

    // Read new Data
    ReadCalibratedAccelerometerData(Data);

    // Look at the roll
    if(Data[0] > 8)
    {
      led += 8; // LED near User-Button
    }
    else if(Data[0] < -8)
    {
      led += 2; // LED near Reset-Button
    }

    // Look at the Pitch
    if(Data[1] > 8)
    {
      led += 4; // LED near Mic
      //volume--;
      if(volume_left>0) volume_left--;
      if(volume_right<100) volume_right++;
    }
    else if(Data[1] < -8)
    {
      led += 1; // LED near CPU
      //volume++;
      if(volume_right>0) volume_right--;
      if(volume_left<100) volume_left++;
    }

    // Set the LEDs
    SetLEDs(led);

  }
}


