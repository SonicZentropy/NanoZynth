#ifndef ZEN_HEADER_H_INCLUDED
#define ZEN_HEADER_H_INCLUDED

#include "JuceHeader.h"

// COMPONENTS
#include "components/AssociatedComponent.h"
#include "components/AssociatedSlider.h"
#include "components/AssociatedButton.h"
#include "components/AssociatedTextButton.h"
// Specific Derived Components
#include "components/derived/GainCtrlSlider.h"

// UTILITIES
#include "utilities/ZenParamUtils.h"
#include "utilities/DecibelConversions.h"
#include "utilities/ZenDebug.h"

// PARAMETERS
#include "parameters/ZenParameter.h"
#include "parameters/FloatParameter.h"
#include "parameters/DecibelParameter.h"
#include "parameters/BooleanParameter.h"

// DSP Processing
#include "processing/BufferSampleProcesses.h"

// Utility Defines

#define S(inText) String(inText)

#endif   // ZEN_HEADER_H_INCLUDED
