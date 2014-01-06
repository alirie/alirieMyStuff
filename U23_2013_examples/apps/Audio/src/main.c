#include <System.h>
#include <stdio.h>
#include <Synth.h>

SynthSong song = {
  Gaming_AudioFreq_22k,
  1,
  &(SynthChannel) {
    SynthRect,
    19*2,
    (SynthNote[]) {
      { Note_a1, 250, 0xff },
      { Note_Pause, 62, 0x00 },
      { Note_a1, 250, 0xff },
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
      { Note_Pause, 62, 0x00 },
    }
  }
};


static void AudioCallback(void *context, int16_t buffer[256])
{
  iprintf("Called AudioCallback()\n");
  for(int i = 0; i < 128; i++) {
    buffer[2*i+0] = buffer[2*i+1] = SynthGetSample(&song);
  }
}



int main(void)
{

  InitializeSystem();

  EnableDebugOutput(DEBUG_ITM);


  InitializeLEDs();

  SetLEDs(1);



  InitializeAudio(Gaming_AudioFreq_22k);

  //iprintf("%i\n", IsAudioInitialized());

  //AudioOn();

  SetAudioVolume(0xff);
      //iprintf("%i\n", 123);
    PlayAudioWithCallback(AudioCallback, NULL);
  
  while(1)
  {


  }
}


