#include <stdio.h>
// #include <unistd.h>
#include <stdlib.h> 
#include <string.h>
#include <iostream>
#include <portaudio.h>
#include <pa_jack.h>
#include "fullBacanoDSP.h"
using namespace std;

char jackName[] = "FullBacano";

// El stream
PaStream *stream;

// El DSP
FullBacanoDSP *dsp;

//==================//
//  Audio Callback  //
//==================//
static int audioCallback( const void* inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData ){
   float* out = (float*)outputBuffer;		
   float* in = (float*)inputBuffer;
   
   return dsp->process(in, out, framesPerBuffer); 
   return 0;
}
//==============//
//  Init Audio  //
//==============//
int getAudioDeviceInfo(){
   PaError err;

   err = Pa_Initialize();
   if(err!= paNoError){
      std::cout << "PortAudio error:" << Pa_GetErrorText(err) << "\n";
      exit(1);
   }

   // Show available devices
   PaDeviceIndex ndev = Pa_GetDeviceCount();
   if(ndev<0){
      std::cout << "PortAudio error:" << Pa_GetErrorText(ndev) << "\n";
      exit(1);      
   }

   std::cout << "\nAudio output devices:\n";
   int i;
   for(i=0; i<ndev; i++){
      const PaDeviceInfo *info = Pa_GetDeviceInfo((PaDeviceIndex) i);
      if(info->maxOutputChannels > 0){
         std::cout << "(" << i << ")" << info->name << "\n";
      } 
   }	

   printf("\nAudio input devices:\n");
   for(i=0; i<ndev; i++){ 
      const PaDeviceInfo *info = Pa_GetDeviceInfo((PaDeviceIndex) i);
      if (info->maxInputChannels > 0){
         std::cout << "(" << i << ") " << info->name << "\n";
      } 
   }	
   printf("\n");

   return 0;
}

void initAudio(int audioInDev, int audioOutDev, int sampleRate, int bufferLength){
   PaError err;


   err = Pa_Initialize();
   if(err!= paNoError){
      printf("PortAudio error: %s", Pa_GetErrorText(err));
      exit(1);
   }


   // Open audio streams.
   PaStreamParameters outParameters;
   memset(&outParameters, '\0', sizeof(outParameters)); // Ponga la var outParameters en su estado inicial (todo ceros)
   outParameters.channelCount = 2;
   outParameters.device = audioOutDev;
   outParameters.sampleFormat = paFloat32;

   PaStreamParameters inParameters;
   memset(&inParameters, '\0', sizeof(inParameters));
   inParameters.channelCount = 2;
   inParameters.device = audioInDev;
   inParameters.sampleFormat = paFloat32;


   err = Pa_OpenStream(&stream, &inParameters, &outParameters, sampleRate, bufferLength, paNoFlag, audioCallback, NULL);
   if(err!= paNoError){
      printf("PortAudio error:%s", Pa_GetErrorText(err));
      exit(1);
   }

   // Start the stream
   err = Pa_StartStream( stream );
   if(err!= paNoError){
      printf("PortAudio error:%s", Pa_GetErrorText(err));
      exit(1);
   }
}

//========//
//  Main  //
//========//
int main(){
   //crear instancia del FullBacanoDSP
   dsp = new FullBacanoDSP();

   PaJack_SetClientName(jackName);
   getAudioDeviceInfo();

   int inputDevice, outputDevice;
   //reading device from standard input
   cout << "Choose your input device:" << endl;
   cin >> inputDevice;
   cout << "Choose your output device: " << endl;
   cin >> outputDevice;
   //initAudio(7,7,48000,256);
   initAudio(inputDevice,outputDevice,48000,128);

   while(1){
      sleep(1);
   }

   delete dsp;
}


