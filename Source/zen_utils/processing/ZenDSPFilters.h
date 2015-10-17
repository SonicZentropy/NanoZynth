/*==============================================================================
//  ZenDSPFilters.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/10/07
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: General DSP ZenDSPFilters (IIR)
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef FILTERS_H_INCLUDED
#define FILTERS_H_INCLUDED

#include "JuceHeader.h"
#include <iostream>
#include "aquila/source/WaveFile.h"

namespace Zen{

	class ZenSimpleFilter
	{

		public:

		ZenSimpleFilter(/*const int numSamples*/);
		~ZenSimpleFilter();

		void processLowPass(const int inNumSamples, float* leftData, float* rightData);

		private:

		int numSamples;
		float prevLeftBuffer = 0;
		float prevRightBuffer = 0;

	};
} // namespace Zen
#endif // FILTERS_H_INCLUDED
