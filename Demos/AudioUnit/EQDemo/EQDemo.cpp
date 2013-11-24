/*
     File: EQDemo.cpp
 Abstract: EQDemo.h
  Version: 1.0
 
*/

// This file defines the EQDemo class, as well as the EQDemoKernel
//  helper class.
#include <stdio.h>
#include "EQDemo.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AUDIOCOMPONENT_ENTRY(AUBaseFactory, EQDemo)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// The COMPONENT_ENTRY macro is required for the Mac OS X Component Manager to recognize and 
// use the audio unit

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	EQDemo::EQDemo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// The constructor for new EQDemo audio units
EQDemo::EQDemo (AudioUnit component) : AUEffectBase (component) {

	// This method, defined in the AUBase superclass, ensures that the required audio unit
	//  elements are created and initialized.
	CreateElements ();
	
	// Invokes the use of an STL vector for parameter access.  
	//  See AUBase/AUScopeElement.cpp
	Globals () -> UseIndexedParameters (kNumberOfParameters);

	// During instantiation, sets up the parameters according to their defaults.
	//	The parameter defaults should correspond to the settings for the default preset.
	SetParameter (kParameter_Lowpass_Frequency, kDefaultValue_Lowpass_Freq);
	SetParameter (kParameter_Lowpass_Q, kDefaultValue_Lowpass_Q);
    SetParameter (kParameter_Highpass_Frequency, kDefaultValue_Highpass_Freq);
	SetParameter (kParameter_Highpass_Q, kDefaultValue_Highpass_Q);
	SetParameter (kParameter_Stage_Type, kDefaultValue_Stage_Type);

	// Also during instantiation, sets the preset menu to indicate the default preset,
	//	which corresponds to the default parameters. It's possible to set this so a
	//	fresh audio unit indicates the wrong preset, so be careful to get it right.
	SetAFactoryPresetAsCurrent (kPresets [kPreset_Default]);
        
	#if AU_DEBUG_DISPATCHER
		mDebugDispatcher = new AUDebugDispatcher (this);
	#endif
}


#pragma mark ____Parameters

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	EQDemo::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Called by the audio unit's view; provides information needed for the view to display the
//  audio unit's parameters
ComponentResult
EQDemo::GetParameterInfo (
		AudioUnitScope			inScope,
		AudioUnitParameterID	inParameterID,
		AudioUnitParameterInfo	&outParameterInfo
) {
	ComponentResult result = noErr;

	// Adds two flags to all parameters for the audio unit, indicating to the host application 
	// that it should consider all the audio unit’s parameters to be readable and writable.
	outParameterInfo.flags = 	  
		kAudioUnitParameterFlag_IsWritable | kAudioUnitParameterFlag_IsReadable;
    
    // All three parameters for this audio unit are in the "global" scope.
	if (inScope == kAudioUnitScope_Global) {
        switch (inParameterID) {
		
            case kParameter_Lowpass_Frequency:
				AUBase::FillInParameterName (
					outParameterInfo,
					kParamName_Lowpass_Freq,
					false
				);
				outParameterInfo.unit			= kAudioUnitParameterUnit_Hertz;
				outParameterInfo.minValue		= kMinimumValue_Lowpass_Freq;
				outParameterInfo.maxValue		= kMaximumValue_Lowpass_Freq;
				outParameterInfo.defaultValue	= kDefaultValue_Lowpass_Freq;
				outParameterInfo.flags		    |= kAudioUnitParameterFlag_DisplayLogarithmic;
				break;

            case kParameter_Lowpass_Q:
				AUBase::FillInParameterName (
					outParameterInfo,
					kParamName_Lowpass_Q,
					false
				);
				//outParameterInfo.unit			= kAudioUnitParameterUnit_CustomUnit;
				outParameterInfo.minValue		= kMinimumValue_Lowpass_Q;
				outParameterInfo.maxValue		= kMaximumValue_Lowpass_Q;
				outParameterInfo.defaultValue	= kDefaultValue_Lowpass_Q;
				break;
            
            case kParameter_Highpass_Frequency:
				AUBase::FillInParameterName (
                                             outParameterInfo,
                                             kParamName_Highpass_Freq,
                                             false
                                             );
				outParameterInfo.unit			= kAudioUnitParameterUnit_Hertz;
				outParameterInfo.minValue		= kMinimumValue_Highpass_Freq;
				outParameterInfo.maxValue		= kMaximumValue_Highpass_Freq;
				outParameterInfo.defaultValue	= kDefaultValue_Highpass_Freq;
				outParameterInfo.flags		    |= kAudioUnitParameterFlag_DisplayLogarithmic;
				break;
                
            case kParameter_Highpass_Q:
				AUBase::FillInParameterName (
                                             outParameterInfo,
                                             kParamName_Highpass_Q,
                                             false
                                             );
				//outParameterInfo.unit			= kAudioUnitParameterUnit_CustomUnit;
				outParameterInfo.minValue		= kMinimumValue_Highpass_Q;
				outParameterInfo.maxValue		= kMaximumValue_Highpass_Q;
				outParameterInfo.defaultValue	= kDefaultValue_Highpass_Q;
				break;
            
            case kParameter_Stage_Type:
			// Invoked when the view needs information for the kTremoloParam_Waveform parameter.
				AUBase::FillInParameterName (
					outParameterInfo,
					kParamName_Stage_Type,
					false
				);
				outParameterInfo.unit			= kAudioUnitParameterUnit_Indexed;
				// Sets the unit of measurement for the Waveform parameter to "indexed," allowing 
				// it to be displayed as a pop-up menu in the generic view. The following three 
				// statements set the minimum, maximum, and default values for the depth parameter. 
				// All three are required for proper functioning of the parameter's user interface.
				outParameterInfo.minValue		= kLowpass_Stage_Type;
				outParameterInfo.maxValue		= kPeak_Stage_Type;
				outParameterInfo.defaultValue	= kDefaultValue_Stage_Type;
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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	EQDemo::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Provides the strings for the Waveform popup menu in the generic view
ComponentResult
EQDemo::GetParameterValueStrings (
	AudioUnitScope			inScope,
	AudioUnitParameterID	inParameterID,
	CFArrayRef				*outStrings
) {
	if ((inScope == kAudioUnitScope_Global) && (inParameterID == kParameter_Stage_Type)) {
	// This method applies only to the waveform parameter, which is in the global scope.
	
		// When this method gets called by the AUBase::DispatchGetPropertyInfo method, which 
		// provides a null value for the outStrings parameter, just return without error.
		if (outStrings == NULL) return noErr;
		
		// Defines an array that contains the pop-up menu item names.
		CFStringRef	strings [] = {
			kMenuItem_Highpass,
			kMenuItem_Lowpass,
            kMenuItem_Peak
		};
   
		// Creates a new immutable array containing the menu item names, and places the array 
		// in the outStrings output parameter.
		*outStrings = CFArrayCreate (
			NULL,
			(const void **) strings,
			(sizeof (strings) / sizeof (strings [0])),
			NULL
		);
		return noErr;
    }
    return kAudioUnitErr_InvalidParameter;
}

#pragma mark ____Properties

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	EQDemo::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult
EQDemo::GetPropertyInfo (
	// This audio unit doesn't define any custom properties, so it uses this generic code for
	// this method.
	AudioUnitPropertyID	inID,
	AudioUnitScope		inScope,
	AudioUnitElement	inElement,
	UInt32				&outDataSize,
	Boolean				&outWritable
) {
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	EQDemo::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult
EQDemo::GetProperty (
	// This audio unit doesn't define any custom properties, so it uses this generic code for
	// this method.
	AudioUnitPropertyID inID,
	AudioUnitScope 		inScope,
	AudioUnitElement 	inElement,
	void				*outData
) {
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

#pragma mark ____Factory Presets

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	EQDemo::GetPresets
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// For users to be able to use the factory presets you define, you must add a generic 
// implementation of the GetPresets method. The code here works for any audio unit that can 
// support factory presets.

// The GetPresets method accepts a single parameter, a pointer to a CFArrayRef object. This 
// object holds the factory presets array generated by this method. The array contains just 
// factory preset numbers and names. The host application uses this array to set up its 
// factory presets menu and when calling the NewFactoryPresetSet method.

ComponentResult
EQDemo::GetPresets(CFArrayRef	*outData) const
{

	// Checks whether factory presets are implemented for this audio unit.
	if (outData == NULL) return noErr;
	
	// Instantiates a mutable Core Foundation array to hold the factory presets.
	CFMutableArrayRef presetsArray = CFArrayCreateMutable (
		NULL,
		kNumberPresets,
		NULL
	);
	
	// Fills the factory presets array with values from the definitions in the TremoloUnit.h 
	// file.
	for (int i = 0; i < kNumberPresets; ++i) {
		CFArrayAppendValue (
			presetsArray,
			&kPresets [i]
		);
    }
    
	// Stores the factory presets array at the outData location.
	*outData = (CFArrayRef) presetsArray;
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	EQDemo::NewFactoryPresetSet
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// The NewFactoryPresetSet method defines all the factory presets for an audio unit. Basically, 
// for each preset, it invokes a series of SetParameter calls.

// This method takes a single argument of type AUPreset, a structure containing a factory
//  preset name and number.
OSStatus
EQDemo::NewFactoryPresetSet(const AUPreset &inNewFactoryPreset)
{
	// Gets the number of the desired factory preset.
	SInt32 chosenPreset = inNewFactoryPreset.presetNumber;
	
	if (
		// Tests whether the desired factory preset is defined.
		chosenPreset == kPreset_Phone || chosenPreset == kPreset_Default
	) {
		// This 'for' loop, and the 'if' statement that follows it, allow for noncontiguous preset 
		// numbers.
		for (int i = 0; i < kNumberPresets; ++i) {
			if (chosenPreset == kPresets[i].presetNumber) {

				//Selects the appropriate case statement based on the factory preset number.
				switch (chosenPreset) {

					// The settings for the "Slow & Gentle" factory preset.
					case kPreset_Phone:
						SetParameter (
							kParameter_Lowpass_Frequency,
							kParameter_Preset_Lowpass_Frequency_Phone
						);
						SetParameter (
							kParameter_Highpass_Frequency,
							kParameter_Preset_Highpass_Frequency_Phone
						);
						SetParameter (
							kParameter_Stage_Type,
							kParameter_Preset_Stage_Type
						);
						break;
					
				}
				
				// Updates the preset menu in the generic view to display the new factory preset.
				SetAFactoryPresetAsCurrent (
					kPresets [i]
				);
				return noErr;
			}
		}
	}
	return kAudioUnitErr_InvalidProperty;
}



#pragma mark ____TremoloUnitEffectKernel

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	EQDemo::EQDemoKernel::EQDemoKernel()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This is the constructor for the TremoloUnitKernel helper class, which holds the DSP code 
//  for the audio unit. TremoloUnit is an n-to-n audio unit; one kernel object gets built for 
//  each channel in the audio unit.
//
// The first line of the method consists of the constructor method declarator and constructor-
//  initializer.
//
// (In the Xcode template, the header file contains the call to the superclass constructor.)
EQDemo::EQDemoKernel::EQDemoKernel (AUEffectBase *inAudioUnit ) : AUKernelBase (inAudioUnit)
{	
	// Gets the samples per second of the audio stream provided to the audio unit. 
	// Obtaining this value here in the constructor assumes that the sample rate
	// will not change during one instantiation of the audio unit.
	mSampleFrequency = GetSampleRate ();
    mLowpass = FTA_RBJFilterInit(RBJ_LPF, 0.0, mSampleFrequency);
    mHighpass = FTA_RBJFilterInit(RBJ_HPF, 20000.0, mSampleFrequency);
    mLowCutoffsmoother = FTA_OnePoleFilterInit(20, mSampleFrequency);
    mHighCutoffsmoother = FTA_OnePoleFilterInit(20, mSampleFrequency);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	EQDemo::EQDemoKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Flush filter buffers
void
EQDemo::EQDemoKernel::Reset()
{
	FTA_RBJFilterFlush(mLowpass);
	FTA_RBJFilterFlush(mHighpass);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	EQDemo::EQDemoKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This method contains the DSP code. 
void
EQDemo::EQDemoKernel::Process (
	const Float32 	*inSourceP,			// The audio sample input buffer.
	Float32		 	*inDestP,			// The audio sample output buffer.
	UInt32 			inSamplesToProcess,	// The number of samples in the input buffer.
	UInt32			inNumChannels,		// The number of input channels. This is always equal to 1 
										//   because there is always one kernel object instantiated
										//   per channel of audio.
	bool			&ioSilence			// A Boolean flag indicating whether the input to the audio 
										//   unit consists of silence, with a TRUE value indicating 
										//   silence.
)
{
	// Ignores the request to perform the Process method if the input to the audio unit is silence.
	if (!ioSilence) {

		// Assigns a pointer variable to the start of the audio sample input buffer.
		const Float32 *sourceP = inSourceP;

		// Assigns a pointer variable to the start of the audio sample output buffer.
		Float32	*destP = inDestP;


        Float32 tempBuffer[inSamplesToProcess];

		// Once per input buffer, get effect params from the user 
		//	via the audio unit view.
		Float32 lowpassFrequency = FTA_OnePoleFilterTick(mLowCutoffsmoother, GetParameter(kParameter_Lowpass_Frequency));
		Float32 lowpassQ = GetParameter (kParameter_Lowpass_Q);
		Float32 highpassFrequency =  FTA_OnePoleFilterTick(mHighCutoffsmoother, GetParameter(kParameter_Highpass_Frequency));
		Float32 highpassQ = GetParameter(kParameter_Highpass_Q);
        //int stageType = (int) GetParameter(kParameter_Stage_Type);

	
		// Performs bounds checking on the parameters.
		if (lowpassFrequency	< kMinimumValue_Lowpass_Freq)
			lowpassFrequency	= kMinimumValue_Lowpass_Freq;
		if (lowpassFrequency	> kMaximumValue_Lowpass_Freq)
			lowpassFrequency	= kMaximumValue_Lowpass_Freq;
        
        if (lowpassQ	< kMinimumValue_Lowpass_Q)
			lowpassQ	= kMinimumValue_Lowpass_Q;
		if (lowpassQ	> kMaximumValue_Lowpass_Q)
			lowpassQ	= kMaximumValue_Lowpass_Q;

		if (highpassFrequency	< kMinimumValue_Highpass_Freq)
			highpassFrequency	= kMinimumValue_Highpass_Freq;
		if (highpassFrequency	> kMaximumValue_Highpass_Freq)
			highpassFrequency	= kMaximumValue_Highpass_Freq;
        
        if (highpassQ	< kMinimumValue_Highpass_Q)
			highpassQ	= kMinimumValue_Highpass_Q;
		if (highpassQ	> kMaximumValue_Highpass_Q)
			highpassQ	= kMaximumValue_Highpass_Q;
	
        // Update Filters
        FTA_RBJFilterSetParams(mLowpass, RBJ_LPF, lowpassFrequency, lowpassQ);
        //FTA_RBJFilterSetCutoff(mLowpass, lowpassFrequency);
        //FTA_RBJFilterSetQ(mLowpass, lowpassQ);
        
        FTA_RBJFilterSetParams(mHighpass, RBJ_HPF, highpassFrequency, highpassQ);
        //FTA_RBJFilterSetCutoff(mHighpass, highpassFrequency);
        //FTA_RBJFilterSetQ(mHighpass, highpassQ);

        // Process available data
        FTA_RBJFilterProcess(mLowpass, tempBuffer, sourceP, inSamplesToProcess);
        FTA_RBJFilterProcess(mHighpass, destP, (const float*)tempBuffer, inSamplesToProcess);

	}
}


