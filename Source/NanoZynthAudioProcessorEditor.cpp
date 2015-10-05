/*==============================================================================
//  NanoZynthAudioProcessorEditor.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/26
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Main GUI Component class
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/


#include "NanoZynthAudioProcessorEditor.h"
#include "zen_utils\debug\ZenDebugEditor.h"

//==============================================================================
NanoZynthAudioProcessorEditor::NanoZynthAudioProcessorEditor(NanoZynthAudioProcessor& ownerFilter)
	: AudioProcessorEditor(ownerFilter)
{
//	DBGM("In NanoZynthAudioProcessorEditor::NanoZynthAudioProcessorEditor() ");
	processor = &ownerFilter;

	// #NOTES: Change Is_Synth in AppConfig.h for Bitwig
    setName("NanoZynthMainComponent");
    addAndMakeVisible (muteButton = new AssociatedTextButton("Mute Button", processor->muteParam));
    muteButton->setTooltip ("Mute all audio");
    muteButton->setButtonText ("MUTE");
	muteButton->setClickingTogglesState(true);
    muteButton->addListener (this);

	
    addAndMakeVisible (gainSlider = new AssociatedSlider ("Gain Slider", processor->audioGainParam, "dB"));
    gainSlider->setTooltip ("Adjusts audio gain");
    gainSlider->setRange (-96, 12, 0.01);
    gainSlider->setSliderStyle (Slider::LinearHorizontal);
    gainSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
	gainSlider->setTextValueSuffix("dB");
	gainSlider->setDoubleClickReturnValue(true, 0.0);
	gainSlider->addListener (this);
	
    addAndMakeVisible (bypassButton = new AssociatedTextButton ("Bypass Button", processor->bypassParam));
    bypassButton->setTooltip ("Bypass Plugin");
    bypassButton->setButtonText ("Bypass");
	bypassButton->setClickingTogglesState(true);
    bypassButton->addListener (this);

    this->setSize (400, 400);
	
	ZEN_COMPONENT_DEBUG_ATTACH(this);


	startTimer(50); // Start timer poll with 50ms rate
}

NanoZynthAudioProcessorEditor::~NanoZynthAudioProcessorEditor()
{
//	DBGM("In NanoZynthAudioProcessorEditor::~NanoZynthAudioProcessorEditor() ");
	ZenDebugEditor::getInstance()->removeComponentDebugger();
    muteButton = nullptr;
    gainSlider = nullptr;
    bypassButton = nullptr;
		
}

//==============================================================================
void NanoZynthAudioProcessorEditor::paint (Graphics& g)
{

    g.fillAll (Colour (0xff303030));
}

void NanoZynthAudioProcessorEditor::resized()
{
//	DBGM("In NanoZynthAudioProcessorEditor::resized() ");
    muteButton->setBounds (10, 6, 74, 24);
    gainSlider->setBounds (158, 8, 150, 24);
    bypassButton->setBounds (10, 38, 74, 24);
}


void NanoZynthAudioProcessorEditor::buttonClicked(Button* buttonThatWasClicked)
{	
//	 DBGM("In NanoZynthAudioProcessorEditor::buttonClicked() ");
	 dynamic_cast<AssociatedButton*>(buttonThatWasClicked)->setAssociatedParameterValueNotifyingHost();
}

void NanoZynthAudioProcessorEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{    		
//	DBGM("In NanoZynthAudioProcessorEditor::sliderValueChanged()");
	dynamic_cast<AssociatedSlider*>(sliderThatWasMoved)->setAssociatedParameterValueNotifyingHost();
}


void NanoZynthAudioProcessorEditor::timerCallback()
{
	//undoManager.beginNewTransaction();
	AssociatedSlider* currentSlider;
	AssociatedButton* currentButton;
	AssociatedComponent* currentComponent;
	ZenParameter* currentParameter;

	int numComponents = this->getNumChildComponents();
	
	//This looks ugly, but it's a whole lot better than a massive if chain checking EVERY param individually
	//This should handle every component's GUI updates automatically
	for (int i = 0; i < numComponents; i++)
	{
		currentComponent = dynamic_cast<AssociatedComponent*>(this->getChildComponent(i));
		if (nullptr == currentComponent) continue;

		currentParameter = currentComponent->getAssociatedParameter();
		if (!currentParameter->needsUIUpdate()) //Don't need to keep going if the Component doesn't need to be updated
		{
			continue;
		}

		currentSlider = dynamic_cast<AssociatedSlider*>(currentComponent);
		if(currentSlider != nullptr)
		{
			currentSlider->setValue(currentParameter->getValueForGUIComponent(), dontSendNotification);
 		} 
		else 
		{
			currentButton = dynamic_cast<AssociatedButton*>(this->getChildComponent(i));
			if (currentButton != nullptr)
			{
				currentButton->setToggleState(currentParameter->getBoolFromValue(), dontSendNotification);
 			}
			else
			{
				DBG("In NanoZynthAudioProcessorEditor::timerCallback() ");
				jassertfalse;
			}
		}
		currentParameter->resetUIUpdate(); //Finished Updating UI
	}
}

