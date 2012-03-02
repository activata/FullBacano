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
   // std::cout << /*  */"Ruido!" << "\n";
   for(unsigned int i = 0; i < bufferSize; i=i+2) {
      distortion->process(in[i], out[i]);
      distortion->process(in[i], out[i+1]);
   }

   return 0;
}


