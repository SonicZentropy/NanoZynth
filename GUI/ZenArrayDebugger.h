/*==============================================================================
//  ZenArrayDebugger.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/25
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Visual window of an array's contents
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef ZENARRAYDEBUGGER_H_INCLUDED
#define ZENARRAYDEBUGGER_H_INCLUDED
#include "JuceHeader.h"


namespace Zen{

class ZenArrayDebugger : public DocumentWindow
{

	/** Display a tree. */
	class Editor :
		public Component, public AudioProcessorListener
	{
	public:
		

		Editor(Array<AudioProcessorParameter*> inArray, AudioProcessor* inProcessor) :
			numParams(inProcessor->getNumParameters())
		{
			
			setSize(1000, 700);
			processor = inProcessor;
			processor->addListener(this);

			int paramIndex = 0;
			
			for(AudioProcessorParameter* param : inArray)
			{
				Label tempLabel(param->getName(500), String(param->getValue()));
				
				//param->
				//tempLabel.setText(param->getName(500) + " " + String(param->getValue()), dontSendNotification );
				tempLabel.setSize(800, 50);
				tempLabel.setBounds(10, tempLabel.getHeight()*paramIndex + 2, tempLabel.getWidth(), tempLabel.getHeight());
			
				addAndMakeVisible(tempLabel);
				paramIndex++;
			}

		}
		~Editor()
		{
			processor->removeListener(this);
			processor = nullptr;
		}

		virtual void audioProcessorParameterChanged(AudioProcessor* processor, int parameterIndex, float newValue) override
		{
			ScopedPointer<Label> tempLabel = static_cast<Label*>(getChildComponent(parameterIndex));
			tempLabel->setText(processor->getParameterName(parameterIndex) + " " + String(newValue), dontSendNotification);
		}
		virtual void audioProcessorChanged(AudioProcessor* processor) override
		{

		}

		void resized() override
		{
			
		}


	public:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Editor);

		ScopedPointer<AudioProcessor> processor;
		unsigned int numParams;
		

	};


public:

	ZenArrayDebugger(OwnedArray<AudioProcessorParameter> inArray, AudioProcessor* inProcessor) :
		DocumentWindow("Value Tree Editor",
			Colours::lightgrey,
			DocumentWindow::allButtons)
	{
		editor = new Editor(inArray, inProcessor);
		setContentNonOwned(editor, true);
		setResizable(true, false);
		setUsingNativeTitleBar(true);
		centreWithSize(getWidth(), getHeight());
		setVisible(true);
	}
	~ZenArrayDebugger()
	{
		//editor->setTree(ValueTree::invalid);
	}

	void closeButtonPressed() override
	{
		setVisible(false);
	}

	void setSource(ValueTree& v)
	{
		//editor->setTree(v);
	}

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueTreeEditor);
	ScopedPointer<Editor> editor;

};
} // namespace Zen
#endif // ZENARRAYDEBUGGER_H_INCLUDED
