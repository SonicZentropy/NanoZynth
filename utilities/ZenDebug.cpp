/* ==============================================================================
//  ZenDebug.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/08
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Static Utility Class for Debug-related code
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#include "ZenDebug.h"
#include "JuceHeader.h"



namespace Zen
{

clock_t ZenDebug::inTime = clock();

int ZenDebug::numSecondsBetweenDebugPrints = 1;

void ZenDebug::setSecondsBetweenDebugPrints(const unsigned int& inSeconds)
{
	numSecondsBetweenDebugPrints = inSeconds;
}

int ZenDebug::getSecondsBetweenDebugPrints()
{
	return numSecondsBetweenDebugPrints;
}

bool ZenDebug::isPrintTimerThresholdReached()
{
	if (((clock() - inTime) / CLOCKS_PER_SEC) > numSecondsBetweenDebugPrints)
	{
		inTime = clock();
		return true;
	}
	return false;
}

void ZenDebug::timedPrint(String inString)
{
	if (isPrintTimerThresholdReached())
	{
		DBG(inString);
	}
}

} // Namespace