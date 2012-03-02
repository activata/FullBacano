#pragma once
#include "portaudio.h"
#include "distortion.h"

class FullBacanoDSP{
   protected:
      // PaStream *outputStream;
      // int outputDevice;
      // int sampleRate;
      // int bufferLength;
      Distortion* distortion;

   public:
      FullBacanoDSP();
      ~FullBacanoDSP();
      void init();
      int process(float *inputBuffer, float *outputBuffer, unsigned long bufferSize);
};
