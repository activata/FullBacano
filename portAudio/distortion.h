#pragma once
#include "portaudio.h"

class Distortion{
   protected:
      float threshold;

   public:
      void setThreshold(float threshold);

      Distortion();
      ~Distortion();
      void init();
      // int process(float *in, float *out, unsigned long bufferSize);
      void process(float &in, float &out);
};
