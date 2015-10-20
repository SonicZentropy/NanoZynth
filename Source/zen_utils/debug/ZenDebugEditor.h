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
	class ZenDebugEditor : public DocumentWindow
	{
	public:

		ZenDebugEditor();
		~ZenDebugEditor();

		/// <summary>
		/// Adds a text label to the main Debug Window
		/// </summary>
		void addTraceLabel(const String& inName, const String& inText);
		
		/// <summary>
		/// Adds a text label to the main Debug Window with text set to the Value passed in
		/// </summary>
		void addTraceLabel(const String& inName, Value& theValue);
		
		/// <summary>
		/// Adds a text label to the main Debug window or updates the value of an existing
		/// label matching inName.
		/// </summary>
		void addOrSetTraceLabel(const String& inName, const String& inText);
		
		/// <summary>
		/// Removes a trace label from the debug window and stops updating.
		/// </summary>
		void removeTraceLabel(const String& inName);

		/// <summary>
		/// Changes the displayed text of the requested label
		/// </summary>
		void setLabelText(const String& labelName, const String& inText);
		
		/// <summary>
		/// Changes the displayed text of the requested label
		/// </summary>
		void setLabelText(const String& labelName, const float inText);		

		void closeButtonPressed() override;

		/// <summary>
		/// Sets the main debug source for display and editing a value tree.  This is normally used
		/// for parameter debugging.
		/// </summary>
		/// <param name="rootTree">The root value tree node to display and manage</param>
		void setSource(ValueTree& rootTree);

		/// <summary>
		/// Attaches a component to the component debugging tab.  This is
		/// basically only for use in a JUCE Editor class
		/// </summary>
		/// <param name="rootComponent"></param>
		void attachComponentDebugger(Component* rootComponent);

		void removeInstanceComponentDebugger();

		void refreshComponentDebugger();
		void resized() override;

		juce_DeclareSingleton(ZenDebugEditor, false);

		static void removeComponentDebugger();
	private:
	
		ScopedPointer<TabbedComponent> tabsComponent;
		ScopedPointer<ValueTreeEditor::Editor> valueTreeEditorComponent;
		ScopedPointer<BufferVisualiser> bufferVisualiserComponent;
		ScopedPointer<ZenMidiVisualiserComponent> midiVisualiserComponent;
		ScopedPointer<ComponentDebugger> componentVisualiserComponent;
		ScopedPointer<NotepadComponent> notepadComponent;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZenDebugEditor);
	};

	//==============================================================================
	// INLINE MACRO FUNCTION TRANSLATIONS
	//==============================================================================

	#ifdef ZEN_DEBUG

	/// <summary>
	/// Adds or updates a text label to the main Debug Window.  This can be updated as often
	/// as desired, but incurs a performance penalty if used in the audio process block.
	/// It will be removed in Release builds entirely.
	/// </summary>
	inline void ZEN_LABEL_TRACE(const String& labelName, const String& labelText)
	{
		ZenDebugEditor::getInstance()->addOrSetTraceLabel(labelName, labelText);
	};

	/// <summary>
	/// Removes an existing trace label from the main debug window
	/// </summary>
	inline void ZEN_REMOVE_LABEL_TRACE(const String& labelName)
	{
		ZenDebugEditor::getInstance()->removeTraceLabel(labelName);
	}

	/// <summary>
	/// Adds a float buffer to the debug buffer visualiser.  This can be added anywhere, including
	/// the processing block, because it is removed from release builds.  Note that it DOES 
	/// incur a performance penalty during debugging that can be severe depending 
	/// on how/where it is used.  In particular, attempting to watch a buffer on a sample-wise basis
	/// is cripplingly slow.
	/// 
	/// Example usage to view the left channel audio buffer entering a process block:
	/// ZEN_DEBUG_BUFFER("Left Audio Buffer: ", leftData, buffer.getNumSamples(), -1, 1);
	/// </summary>
	/// <param name="name"></param>
	/// <param name="data"></param>
	/// <param name="size"></param>
	/// <param name="min"></param>
	/// <param name="max"></param>
	inline void ZEN_DEBUG_BUFFER(const String & name, float * data, int size, float min, float max)
	{
		Store::getInstance()->record(name, data, size, min, max);
	}
	
	/// <summary>
	/// Create a Component Debugger tab and attach the parameter component to it.
	/// </summary>
	/// <param name="rootComponent">Base Component for the debug window</param>
	inline void ZEN_COMPONENT_DEBUG_ATTACH(Component* rootComponent)
	{
		ZenDebugEditor::getInstance()->attachComponentDebugger(rootComponent);
	}

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
