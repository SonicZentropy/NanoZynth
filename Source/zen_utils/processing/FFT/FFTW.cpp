/*==============================================================================
//  FFTW.cpp
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

#include "FFTW.h"

namespace Zen
{

	FFTW::FFTW(const unsigned int timeDomainSize, const bool forwardDirection) :
		size(timeDomainSize), realBuffer(nullptr), complexBuffer(nullptr), plan(nullptr)
	{
		realBuffer = static_cast<float*>(fftwf_malloc(sizeof(float)*size));
		if (realBuffer == nullptr) throw std::bad_alloc();

		//complexBuffer =
	}
	
	FFTW::~FFTW()
	{
	
	}

}

