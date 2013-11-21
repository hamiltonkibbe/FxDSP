/*
     File: EQDemoVersion.h
 Abstract: EQDemoVersion.h
  Version: 1.0
 
*/

#ifndef __EQDemoVersion_h__
#define __EQDemoVersion_h__


#ifdef DEBUG
	#define kEQDemoVersion 0xFFFFFFFF
#else
	#define kEQDemoVersion 0x00010000
#endif

// customized for each audio unit
#define EQDemo_COMP_SUBTYPE		'EQUA'
#define EQDemo_COMP_MANF		'FTAu'

#endif

