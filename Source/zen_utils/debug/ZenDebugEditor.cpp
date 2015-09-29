/*==============================================================================
//  ZenDebugEditor.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/28
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Implementation for ZenDebugEditor.h
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#include "ZenDebugEditor.h"

#ifdef ZEN_DEBUG

namespace Zen
{

ValueTreeEditor* ZenDebugEditor::getInstance()
{
	if (!ZenDebugEditor::editorInstance)
		editorInstance = new ValueTreeEditor();

	return editorInstance;
}

ValueTreeEditor* ZenDebugEditor::editorInstance = nullptr;
} // namespace Zen

#endif // JUCE_DEBUG