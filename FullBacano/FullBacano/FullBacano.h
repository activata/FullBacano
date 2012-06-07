//File:		FullBacano.h


#include "AUEffectBase.h"
#include "FullBacanoVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __FullBacano_h__
#define __FullBacano_h__


#pragma mark ____FullBacano Parameters
// parameters


static CFStringRef kParameterName_th = CFSTR("Threshold");
static const float kDefaultValue_th = 0.027;
static const float kMinimumValue_th   = 0.0;                    // 0
static const float kMaximumValue_th = 1.0;   

static CFStringRef kParameterName_Gain = CFSTR("Gain");
static const float kDefaultValue_Gain = 7.1;
static const float kMinimumValue_Gain   = 0.0;                    // 1
static const float kMaximumValue_Gain = 100;   

static CFStringRef kParameterName_Freq = CFSTR("Frecuency");
static const float kDefaultValue_Freq = 151.2;
static const float kMinimumValue_Freq   = 0.0;                    // 2
static const float kMaximumValue_Freq = 800;   

static CFStringRef kParameterName_Q = CFSTR("Q Factor");
static const float kDefaultValue_Q = 15;
static const float kMinimumValue_Q   = 0.1;                    // 3
static const float kMaximumValue_Q = 15;   

static CFStringRef kParameterName_Matrix  = CFSTR ("Out Matrix");     // 6

static const int kLowDist_Matrix     = 1;
static const int kHiDist_Matrix   = 2;
static const int kLowPass_Matrix     = 3;
static const int kHiPass_Matrix   = 4;
static const int kFullPass_Matrix   = 5;
static const int kFullDist_Matrix   = 6;

static const int kDefaultValue_Matrix= kFullPass_Matrix;

// menu item names for the Matrix parameter
static CFStringRef kMenuItem_DistLow = CFSTR ("Dist Lo");         // 7
static CFStringRef kMenuItem_DistHi = CFSTR ("Dist Hi ");
static CFStringRef kMenuItem_LowPass = CFSTR ("Low Pass+Dist Lo");         // 7
static CFStringRef kMenuItem_HiPass = CFSTR ("Hi Pass+Dist Hi");
static CFStringRef kMenuItem_FullPass = CFSTR ("FullBacano");
static CFStringRef kMenuItem_FullDist = CFSTR ("Full Distorted");
enum {
	kParam_th =0,
  kParam_Gain =1,
  kParam_Freq =2,
  kParam_Q =3,
  kParam_Matrix=4,
	kNumberOfParameters=5
};

#pragma mark ____FullBacano
class FullBacano : public AUEffectBase
{
public:
	FullBacano(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~FullBacano () { delete mDebugDispatcher; }
#endif
	
	virtual AUKernelBase *		NewKernel() { return new FullBacanoKernel(this); }
	
	virtual	OSStatus			GetParameterValueStrings(AudioUnitScope			inScope,
														 AudioUnitParameterID		inParameterID,
														 CFArrayRef *			outStrings);
    
	virtual	OSStatus			GetParameterInfo(AudioUnitScope			inScope,
												 AudioUnitParameterID	inParameterID,
												 AudioUnitParameterInfo	&outParameterInfo);
    
	virtual OSStatus			GetPropertyInfo(AudioUnitPropertyID		inID,
												AudioUnitScope			inScope,
												AudioUnitElement		inElement,
												UInt32 &			outDataSize,
												Boolean	&			outWritable );
	
	virtual OSStatus			GetProperty(AudioUnitPropertyID inID,
											AudioUnitScope 		inScope,
											AudioUnitElement 		inElement,
											void *			outData);
	
 	virtual	bool				SupportsTail () { return false; }
	
	/*! @method Version */
	virtual OSStatus		Version() { return kFullBacanoVersion; }
	
    
	
protected:
		class FullBacanoKernel : public AUKernelBase		// most real work happens here
	{
public:
		FullBacanoKernel(AUEffectBase *inAudioUnit )
		: AUKernelBase(inAudioUnit)
	{
	}
		
		// *Required* overides for the process method for this effect
		// processes one channel of interleaved samples
        virtual void 		Process(	const Float32 	*inSourceP,
										Float32		 	*inDestP,
										UInt32 			inFramesToProcess,
										UInt32			inNumChannels,
										bool			&ioSilence);
		
        virtual void		Reset();
		
  private: 
    
    Float32 inputSample,outputSample, distortedSample,LPSample,HPSample,EvalSample,threshold ;    
    Float32 samplingRate,gain,distortionGain,cutFrecuency ;
    Float32 alpha,pI,Q,w0,b0,b1,b2,a0,a1,a2,x1,x2,y1,y2;
    int outMatrix;

	};
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif