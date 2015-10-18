/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include <iostream>
#include "JuceHeader.h"


using namespace std;

//Dump text here to std out and DBG both, so you can see it whether running w/ debugger or not
#define PRT(prtText)  MACRO_WITH_FORCED_SEMICOLON (std::cout << prtText << std::endl; DBG(prtText);)
//==============================================================================
int main()
{		
	Identifier tempID("TestIDTempName");
	
	PRT("Test for TempID: " + tempID.toString());
	return 0;
}

