
/*==============================================================================
//  NanoSynth.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/27
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Main Synth Class
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef NANOSYNTH_H_INCLUDED
#define NANOSYNTH_H_INCLUDED
#include "JuceHeader.h"
#include "SynthFunctions.h"

namespace Zen{

class NanoSynth
{

public:
	
	NanoSynth();

	~NanoSynth();

private:
	unsigned int midiRxChannel;

};
} // namespace Zen
#endif // NANOSYNTH_H_INCLUDED