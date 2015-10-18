/*==============================================================================
//  FFTW.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/10/18
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Handles FFT Performance Processing via wrapping FFTW3
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef FFTW_H_INCLUDED
#define FFTW_H_INCLUDED
#include "JuceHeader.h"
#include "fftw3.h"

namespace Zen
{

	class FFTW
	{

	public:

		FFTW(const unsigned int timeDomainSize, const bool forwardDirection);
		~FFTW();

		inline void executeFFT()
		{
			fftwf_execute(plan);
		}

	private:
		int size;
		float* realBuffer;
		FFT::Complex* complexBuffer;
		fftwf_plan plan;

	};
} // namespace Zen
#endif // FFTW_H_INCLUDED
