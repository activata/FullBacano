
/*=============================================================================
	FullBacano.cpp
	
=============================================================================*/
#include "FullBacano.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(FullBacano)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FullBacano::FullBacano
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FullBacano::FullBacano(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_th, kDefaultValue_th );
  SetParameter(kParam_Gain, kDefaultValue_Gain );
  SetParameter(kParam_Freq, kDefaultValue_Freq);
  SetParameter(kParam_Q, kDefaultValue_Q );
  SetParameter(kParam_Matrix, kDefaultValue_Matrix );        
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


/*//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FullBacano::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus			FullBacano::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}
*/


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FullBacano::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus			FullBacano::GetParameterInfo(AudioUnitScope		inScope,
                                           AudioUnitParameterID	inParameterID,
                                           AudioUnitParameterInfo	&outParameterInfo )
{
	OSStatus result = noErr;
  
	outParameterInfo.flags = 	kAudioUnitParameterFlag_IsWritable
  |		kAudioUnitParameterFlag_IsReadable;
  
  if (inScope == kAudioUnitScope_Global) {
    switch(inParameterID)
    {
      case kParam_th:
        AUBase::FillInParameterName (outParameterInfo, kParameterName_th, false);
        outParameterInfo.unit = kAudioUnitParameterUnit_LinearGain;
        outParameterInfo.minValue =  kMinimumValue_th;
        outParameterInfo.maxValue = kMaximumValue_th;
        outParameterInfo.defaultValue = kDefaultValue_th;
        break;
        
      case kParam_Gain:
        AUBase::FillInParameterName (outParameterInfo, kParameterName_Gain, false);
        outParameterInfo.unit = kAudioUnitParameterUnit_Percent;
        outParameterInfo.minValue =  kMinimumValue_Gain;
        outParameterInfo.maxValue = kMaximumValue_Gain;
        outParameterInfo.defaultValue = kDefaultValue_Gain;
        break;
        
        
      case kParam_Freq:
        AUBase::FillInParameterName (outParameterInfo, kParameterName_Freq, false);
        outParameterInfo.unit = kAudioUnitParameterUnit_Hertz;
        outParameterInfo.minValue =  kMinimumValue_Freq;
        outParameterInfo.maxValue = kMaximumValue_Freq;
        outParameterInfo.defaultValue = kDefaultValue_Freq;
        //outParameterInfo.flags = kAudioUnitParameterFlag_DisplayLogarithmic; //???
        break;
        
      case kParam_Q:
        AUBase::FillInParameterName (outParameterInfo, kParameterName_Q, false);
        outParameterInfo.unit = kAudioUnitParameterUnit_LinearGain;
        outParameterInfo.minValue =  kMinimumValue_Q;
        outParameterInfo.maxValue = kMaximumValue_Q;
        outParameterInfo.defaultValue = kDefaultValue_Q;
        break;
        
      case kParam_Matrix:
        AUBase::FillInParameterName (outParameterInfo, kParameterName_Matrix, false);
        outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
        outParameterInfo.minValue =  kLowDist_Matrix ;
        outParameterInfo.maxValue = kFullDist_Matrix;
        outParameterInfo.defaultValue = kDefaultValue_Matrix;
        break;
        
      default:
        result = kAudioUnitErr_InvalidParameter;
        break;
    }
	} else {
    result = kAudioUnitErr_InvalidParameter;
  }
  
  
  
	return result;
}

ComponentResult FullBacano::GetParameterValueStrings (
                                                      AudioUnitScope          inScope,
                                                      AudioUnitParameterID    inParameterID,
                                                      CFArrayRef              *outStrings
                                                      ) {
  if ((inScope == kAudioUnitScope_Global) &&             // 1
      (inParameterID == kParam_Matrix)) {
    
    if (outStrings == NULL) return noErr;              // 2
    
    CFStringRef strings [] = {                         // 3
      kMenuItem_DistLow,
      kMenuItem_DistHi,
      kMenuItem_LowPass,
      kMenuItem_HiPass,
      kMenuItem_FullPass,
      kMenuItem_FullDist
    };
    
    *outStrings = CFArrayCreate (                      // 4
                                 NULL,
                                 (const void **) strings,
                                 (sizeof (strings) / sizeof (strings [0])),     // 5
                                 NULL
                                 );
    return noErr;
  }
  return kAudioUnitErr_InvalidParameter;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FullBacano::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus			FullBacano::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FullBacano::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus			FullBacano::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{

	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}


#pragma mark ____FullBacanoEffectKernel


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FullBacano::FullBacanoKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		FullBacano::FullBacanoKernel::Reset()
{
  
  samplingRate = GetSampleRate();
  pI = 3.1415926536;
  x1 = 0,x2 = 0,y1 = 0,y2 = 0;  //Filter Coefficients reset
  
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FullBacano::FullBacanoKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		FullBacano::FullBacanoKernel::Process(	const Float32 	*inSourceP,
                                              Float32		 	*inDestP,
                                              UInt32 			inFramesToProcess,
                                              UInt32			inNumChannels, // for version 2 AudioUnits inNumChannels is always 1
                                              bool			&ioSilence )
{
  
	//This code will pass-thru the audio data.
	//This is where you want to process data to produce an effect.
  
	
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	cutFrecuency=GetParameter(kParam_Freq);
  gain = (GetParameter( kParam_Gain ))/100;
	Q=GetParameter(kParam_Q);
  threshold=GetParameter(kParam_th);
  outMatrix=GetParameter(kParam_Matrix);
  w0 = 2*pI*(cutFrecuency/samplingRate);
  alpha = sinf(w0)/(2*Q); 
  b0 = (1 - cosf(w0))/2;
  b1 = 1-cosf(w0);
  b2 = (1-cosf(w0))/2;
  a0 = 1+alpha;
  a1 = -2*cosf(w0);        
  a2 = 1-alpha;
  
	while (nSampleFrames-- > 0) 
  {
		//INPUT
    
    inputSample = *sourceP;
		sourceP += inNumChannels;	// advance to next frame			
    //DSP work
    
    // Low Pass Filter
    LPSample = (b0/a0)*inputSample + (b1/a0)*x1 + (b2/a0)*x2 - (a1/a0)*y1 - (a2/a0)*y2;
    y2=y1;
    y1=LPSample;            
    x2=x1;
    x1=inputSample;
    
    //CrossOver for this fiter 
    //HP Pass = 180 deg LowPass + inputSample
    HPSample = inputSample - LPSample;
    
    // IN Matrix
    switch (outMatrix) 
    {
      case 1: case 3: case 5://Low dist Low Pass  Full Bacano
        EvalSample = LPSample;
        break;
        
      case 2: case 4: case 6://Hi dist Hi pass y full dist
        EvalSample = HPSample;
        
        
      default:
        break;
    }
    
    //Distortion      
    if(fabs(EvalSample) >= threshold)            
    {
      if(EvalSample>0)
        
        distortedSample=1;
      if(EvalSample<0)
      {
        distortedSample = -1;
      }
    }
    
    // DSP Out Matrix
    
    switch (outMatrix) 
    {
      case 1: case 2: 
        outputSample = (distortedSample*gain);
        break;
      case 3: case 6: 
        outputSample = (distortedSample*gain)+LPSample;
        break; 
      case 4: 
        outputSample = (distortedSample*gain)+HPSample;
        
      case 5: 
        outputSample = (distortedSample*gain)+inputSample;
        break;
        
        
      default:
        break;
    }
    
   	*destP = outputSample;
		destP += inNumChannels;
    
  }
}
