/*==============================================================================
//  ZenDSPFilters.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 10/7/2015
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: DSP Filter Implementations
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#include "ZenDSPFilters.h"

namespace Zen
{
	ZenSimpleFilter::ZenSimpleFilter(/*const int inNumSamples*/)
	{
		//numSamples = inNumSamples;
		//Aquila::WaveFile wav("D:/Temp/wavTest.wav");

		/*DBG("Filename: " << wav.getFilename());
		DBG("\nLength: " << wav.getAudioLength() << " ms");
		DBG("\nSample frequency: " << wav.getSampleFrequency() << " Hz");
		DBG("\nChannels: " << S(wav.getChannelsNum()));
		DBG("\nByte rate: " << wav.getBytesPerSec() / 1024 << " kB/s");
		DBG("\nBits per sample: " << S(wav.getBitsPerSample()) << "b\n");*/
	}

	ZenSimpleFilter::~ZenSimpleFilter() {}

	void ZenSimpleFilter::processLowPass(const int inNumSamples, float* leftData, float* rightData)
	{
		for (int currSample = 0; currSample < inNumSamples; ++currSample)
		{
			float leftCurr = leftData[currSample] + prevLeftBuffer;
			//float rightCurr = rightData[currSample] + prevRightBuffer;
			prevLeftBuffer = leftData[currSample];
			//prevRightBuffer = rightData[currSample];
			leftData[currSample] = leftCurr;
			//rightData[currSample] = rightCurr;
		}
	}
}