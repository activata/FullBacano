#include "fullBacanoDSP.h"
#include <iostream>

FullBacanoDSP::FullBacanoDSP(){
   this->init();
};

FullBacanoDSP::~FullBacanoDSP(){
   delete distortion;
};

void FullBacanoDSP::init(){
   distortion = new Distortion();  
}

int FullBacanoDSP::process(float *in, float *out, unsigned long bufferSize){
   // Como no hacer hard-code de este 2 (numCanales)
   for(unsigned long i = 0; i < 2*bufferSize; i++) {
      distortion->process(in, out);
      out++;
      in++;
      distortion->process(in, out);
      out++;
      in++;
   }

   return 0;
}


