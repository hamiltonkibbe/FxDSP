/*
     File: EQDemo.h
 Abstract: EQDemo.h
  Version: 1.0
 

 
*/

#include "AUEffectBase.h"
#include "EQDemoVersion.h"
#include "FtAudioRBJFilter.h"
#include "FtAudioOnePoleFilter.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif

#ifndef __EQDemo_h__
#define __EQDemo_h__

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Constants for parameters and  factory presets
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma mark ____EQDemo Parameter Constants

// Provides the user interface name for the Frequency parameter.
static CFStringRef kParamName_Lowpass_Freq      = CFSTR ("Lowpass Frequency");
static const float kDefaultValue_Lowpass_Freq	= 40.0;
static const float kMinimumValue_Lowpass_Freq	= 20.0;
static const float kMaximumValue_Lowpass_Freq	= 20000.0;

static CFStringRef kParamName_Lowpass_Q         = CFSTR ("Lowpass Q");
static const float kDefaultValue_Lowpass_Q      = 0.71;
static const float kMinimumValue_Lowpass_Q      = 0.1;
static const float kMaximumValue_Lowpass_Q      = 18.0;

static CFStringRef kParamName_Highpass_Freq     = CFSTR ("Highpass Frequency");
static const float kDefaultValue_Highpass_Freq	= 10000.0;
static const float kMinimumValue_Highpass_Freq	= 20.0;
static const float kMaximumValue_Highpass_Freq	= 20000.0;

static CFStringRef kParamName_Highpass_Q         = CFSTR ("Highpass Q");
static const float kDefaultValue_Highpass_Q      = 0.71;
static const float kMinimumValue_Highpass_Q      = 0.1;
static const float kMaximumValue_Highpass_Q      = 18.0;

static CFStringRef kParamName_Stage_Type	= CFSTR ("Stage Type");
static const int kLowpass_Stage_Type		= 1;
static const int kHighpass_Stage_Type       = 2;
static const int kPeak_Stage_Type           = 3;
static const int kDefaultValue_Stage_Type	= kPeak_Stage_Type;

// Defines menu item names for the waveform parameter
static CFStringRef kMenuItem_Lowpass		= CFSTR ("Lowpass");
static CFStringRef kMenuItem_Highpass		= CFSTR ("Highpass");
static CFStringRef kMenuItem_Peak           = CFSTR ("Peak");

// Defines constants for identifying the parameters; defines the total number 
//  of parameters.
enum {
	kParameter_Lowpass_Frequency	= 0,
	kParameter_Lowpass_Q            = 1,
	kParameter_Highpass_Frequency	= 2,
    kParameter_Highpass_Q           = 3,
    kParameter_Stage_Type           = 4,
	kNumberOfParameters             = 5
};

#pragma mark ____EQDemo Factory Preset Constants

// Telephone preset
static const float kParameter_Preset_Lowpass_Frequency_Phone	= 300;
static const float kParameter_Preset_Highpass_Frequency_Phone	= 3000.0;
static const float kParameter_Preset_Stage_Type	= kPeak_Stage_Type;


enum {
	// Defines a constant for the "Slow & Gentle" factory preset.
	kPreset_Phone	= 0,

	// Defines a constant representing the total number of factory presets.
	kNumberPresets	= 1
};

// Defines an array containing two Core Foundation string objects. The objects contain 
//  values for the menu items in the user interface corresponding to the factory presets.
static AUPreset kPresets [kNumberPresets] = {
	{kPreset_Phone, CFSTR ("Telephone")}
};

// Defines a constant representing the default factory preset, in this case the 
//  "Telephone" preset.
static const int kPreset_Default = kPreset_Phone;



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TremoloUnit class
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma mark ____EQDemo
class EQDemo : public AUEffectBase {

public:
	EQDemo (AudioUnit component);
	
#if AU_DEBUG_DISPATCHER
	virtual ~EQDemo () {delete mDebugDispatcher;}
#endif
	
	virtual AUKernelBase *NewKernel () {return new EQDemoKernel(this);}
	
	virtual	ComponentResult GetParameterValueStrings (
		AudioUnitScope			inScope,
		AudioUnitParameterID	inParameterID,
		CFArrayRef				*outStrings
	);
    
	virtual	ComponentResult GetParameterInfo (
		AudioUnitScope			inScope,
		AudioUnitParameterID	inParameterID,
		AudioUnitParameterInfo	&outParameterInfo
	);
    
	virtual ComponentResult GetPropertyInfo (
		AudioUnitPropertyID		inID,
		AudioUnitScope			inScope,
		AudioUnitElement		inElement,
		UInt32					&outDataSize,
		Boolean					&outWritable
	);
	
	virtual ComponentResult GetProperty (
		AudioUnitPropertyID		inID,
		AudioUnitScope			inScope,
		AudioUnitElement		inElement,
		void					*outData
	);
	
 	// report that the audio unit supports the 
	//	kAudioUnitProperty_TailTime property
	virtual	bool SupportsTail () {return true;}
	
	// provide the audio unit version information
	virtual ComponentResult	Version () {return kEQDemoVersion;}

    // Declaration for the GetPresets method (for setting up the factory presets), 
	//  overriding the method from the AUBase superclass.
    virtual ComponentResult	GetPresets (
		CFArrayRef	*outData
	) const;
	
	// Declaration for the NewFactoryPresetSet method (for setting a factory preset 
	//  when requested by the host application), overriding the method from the 
	//  AUBase superclass.
    virtual OSStatus NewFactoryPresetSet (
		const AUPreset	&inNewFactoryPreset
	);

protected:
	class EQDemoKernel : public AUKernelBase {
		public:
			EQDemoKernel (AUEffectBase *inAudioUnit);
			
			// *Required* overides for the process method for this effect
			// processes one channel of interleaved samples
			virtual void Process (
				const Float32 	*inSourceP,
				Float32		 	*inDestP,
				UInt32 			inFramesToProcess,
				UInt32			inNumChannels, // equal to 1
				bool			&ioSilence
		);
		
        virtual void Reset ();
		
		private:
            FTA_RBJFilter    *mLowpass;
            FTA_RBJFilter    *mHighpass;
            FTA_OnePoleFilter *mLowCutoffsmoother;
            FTA_OnePoleFilter *mHighCutoffsmoother;
            Float32             mSampleFrequency;
        
	};
};

#endif
