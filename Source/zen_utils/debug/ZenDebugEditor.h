/*==============================================================================
//  ZenDebugEditor.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/28
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Zen Debug Component for window - contains Value Tree Parameter tab,
//  Buffer Visualization Tab, and Labels tab
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/
#ifndef ZENDEBUGCOMPONENT_H_INCLUDED
#define ZENDEBUGCOMPONENT_H_INCLUDED
#include "JuceHeader.h"
#include "GUI/value_tree_editor.h"

namespace Zen
{

#ifdef ZEN_DEBUG

class ZenDebugEditor
{
public:
	
	static ValueTreeEditor* getInstance();

	static ValueTreeEditor* getInstance(int componentWidth, int componentHeight);
private:
	static ValueTreeEditor* editorInstance;
	
};

inline void ZEN_LABEL_TRACE(const String& labelName, const String& labelText)
{
	ZenDebugEditor::getInstance()->addOrSetTraceLabel(labelName, labelText);
};

inline void ZEN_REMOVE_LABEL_TRACE(const String& labelName)
{
	ZenDebugEditor::getInstance()->removeTraceLabel(labelName);
}

inline void ZEN_DEBUG_BUFFER(const String & name, float * data, int size, float min, float max)
{
	Store::getInstance()->record(name, data, size, min, max);
}

/*
inline void ZEN_COMPONENT_DEBUG_ATTACH(Component* rootComponent)
{
	
}*/

#else
inline void ZEN_LABEL_TRACE(const String& labelName, const String& labelText)
{};

inline void ZEN_REMOVE_LABEL_TRACE(const String& labelName)
{};

inline void ZEN_DEBUG_BUFFER(const String & name, float * data, int size, float min, float max)
{};

inline void ZEN_COMPONENT_DEBUG_ATTACH(Component* rootComponent)
{};


#endif // ZEN_DEBUG
} // Namespace Zen
#endif // ZENDEBUGCOMPONENT_H_INCLUDED
