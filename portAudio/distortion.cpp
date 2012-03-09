#include "distortion.h"
#include <iostream>
#include <math.h>

Distortion::Distortion(){
   this->init();
   this->threshold = 0.3;
};

Distortion::~Distortion(){
};

void Distortion::init(){
}

void Distortion::setThreshold(float threshold){
   if(threshold >= 1.0) this->threshold = 1.0;
   else if(threshold <= -1.0) this->threshold = -1.0;
   else this->threshold=threshold;
}

// int Distortion::process(float *in, float *out, unsigned long bufferSize){ for (unsigned int i=0; i<bufferSize; i++) {
//       out[i] = this->process(in[i]);
//    }
// 
//    return 0;
// }

void Distortion::process(float *in, float *out){
   if(fabs(*in) >= this->threshold){ // podria ser simplemente threshold en lugar de this->threshold
      if(*in > 0){
         *out = 1;
      }
      else{
         *out = -1;
      }
   }
   else{
      *out = *in;
   }
}
