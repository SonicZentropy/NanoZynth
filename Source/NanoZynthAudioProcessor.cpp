/* ==============================================================================
//  NanoZynthAudioProcessor.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/08
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Main DSP Processing Class
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/
#pragma warning(disable:4100)

#include "NanoZynthAudioProcessor.h"
#include "NanoZynthAudioProcessorEditor.h"
#include "zen_utils\utilities\ZenUtils.h"
#include "JuceHeader.h"

// #ENHANCE: add JUCE_TRACK_OBJECT macro to bottom of my params/components

//==============================================================================
	NanoZynthAudioProcessor::NanoZynthAudioProcessor()
		:rootTree("Root")
	{			
		//_crtBreakAlloc = 307;     //Break on this memory allocation number (When Debug)
//		DBGM("In NanoZynthAudioProcessor::NanoZynthAudioProcessor() ");
		addParameter(audioGainParam = new DecibelParameter("Gain", true, 0.01f, -96.0f, 12.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.01f, "dB"));
 		addParameter(muteParam = new BooleanParameter("Mute", false));
		addParameter(bypassParam = new BooleanParameter("Bypass", false));	
		
		rootTree = createParameterTree();
		debugTreeEditor = new jcf::ValueTreeEditor();
		debugTreeEditor->setSize(450, 300);
		debugTreeEditor->setTopLeftPosition(1900 - debugTreeEditor->getWidth(), 1040 - debugTreeEditor->getHeight());
		debugTreeEditor->setSource(rootTree);

	}

	NanoZynthAudioProcessor::~NanoZynthAudioProcessor()
	{
//		DBGM("In NanoZynthAudioProcessor::~NanoZynthAudioProcessor() ");
		rootTree.removeAllChildren(nullptr);
		audioGainParam = nullptr;
		muteParam = nullptr;
		bypassParam = nullptr;

		debugTreeEditor = nullptr;
		
	}
	
	//==============================================================================
	/**Note that if you have more outputs than inputs, then only those channels that 
	correspond to an input channel are guaranteed to contain sensible data - 
	e.g. in the case of 2 inputs and 4 outputs, the first two channels contain the input, 
	but the last two channels may contain garbage, so you should be careful NOT to let 
	this pass through without being overwritten or cleared.*/
	void NanoZynthAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
	{
		setCurrentSampleRate(getSampleRate());
		// #TODO: ADDING MIDI SYNTH FROM DEMO
		
		jcf::JCF_DEBUG_BUFFER("Midi Buffer", midiMessages., 1024, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
		jassert(currentSampleRate >= 0);

		if (bypassParam->isOn()) return;

		float* leftData = buffer.getWritePointer(0);  //leftData references left channel now
		float* rightData = buffer.getWritePointer(1); //right data references right channel now		

		if (muteParam->isOn())
		{
			
			for (int i = 0; i < buffer.getNumSamples() ; i++)
			{
				leftData[i] = rightData[i] = 0;
			}
			return;
		}
		
		//Main Processing Loop
		for (long i = 0; i < buffer.getNumSamples(); i++)
		{
			float audioGainRaw = getClamped(audioGainParam->getSmoothedRawDecibelGainValue(), 0, 4.0f); //Make sure screwups don't blow up speakers
			jassert(audioGainRaw >= 0);
			BufferSampleProcesses::processGain(&leftData[i], &rightData[i], audioGainRaw);
		}
		
		
	}
	
	void NanoZynthAudioProcessor::getStateInformation(MemoryBlock& destData)
	{
//		DBGM("In NanoZynthAudioProcessor::getStateInformation() ");
		// You should use this method to store your parameterSet in the memory block.
		// You could do that either as raw data, or use the XML or ValueTree classes
		// as intermediaries to make it easy to save and load complex data.
		// You should use this method to store your parameters in the memory block.
		// You could do that either as raw data, or use the XML or ValueTree classes
		// as intermediaries to make it easy to save and load complex data.
		
		XmlElement rootXML("Root");

		for (auto param : getParameters())
		{
			ZenParameter* zenParam = dynamic_cast<ZenParameter*>(param);
			zenParam->writeToXML(rootXML);
		}
		//DBG(rootXML.createDocument("", false, false, "UTF-8", 120));
		copyXmlToBinary(rootXML, destData);
	}

	void NanoZynthAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
	{
		// You should use this method to restore your parameters from this memory block,
		// whose contents will have been created by the getStateInformation() call.
//		DBGM("In NanoZynthAudioProcessor::setStateInformation() ");

		
		ScopedPointer<XmlElement> theXML = this->getXmlFromBinary(data, sizeInBytes);
		//DBG(theXML->createDocument("", false, false, "UTF-8", 120));

		if (theXML != nullptr)
		{						
			for (auto param : getParameters())
			{
				ZenParameter* zenParam = dynamic_cast<ZenParameter*>(param);
				if(zenParam != nullptr) zenParam->setFromXML(*theXML);
			}
		}
	}

	ValueTree NanoZynthAudioProcessor::createParameterTree()
	{
		ValueTree valTree("Parameters");

		for (auto param : getParameters())
		{
			ZenParameter* zenParam = dynamic_cast<ZenParameter*>(param);
			if(zenParam != nullptr) valTree.addChild(zenParam->getValueTree(), -1, nullptr);
		}	
		//DBG("Value Tree result from createParameterTree() : " + valTree.toXmlString());
		return valTree;

	}

#pragma region overrides
	//==============================================================================
	void NanoZynthAudioProcessor::prepareToPlay(double inSampleRate, int samplesPerBlock)
	{
//		DBGM("In NanoZynthAudioProcessor::prepareToPlay() ");
		// Use this method as the place to do any pre-playback
		// initialisation that you need..
	
		// Iterates over parameters and resets Smooth for the ones who need it
		for (auto param : getParameters())
		{			
			ZenParameter* zenParam = dynamic_cast<ZenParameter*>(param);
			if (zenParam != nullptr)
			{
				if (zenParam->checkShouldBeSmoothed())
				{
					zenParam->resetSmoothedValue(inSampleRate);
				}
			}
		}
	}

	//==============================================================================
	const String NanoZynthAudioProcessor::getName() const
	{
		return JucePlugin_Name;
	}

	const String NanoZynthAudioProcessor::getInputChannelName(int channelIndex) const
	{
		return String(channelIndex + 1);
	}


	const String NanoZynthAudioProcessor::getOutputChannelName(int channelIndex) const
	{
		return String(channelIndex + 1);
	}

	bool NanoZynthAudioProcessor::isInputChannelStereoPair(int index) const
	{
		return true;
	}

	bool NanoZynthAudioProcessor::isOutputChannelStereoPair(int index) const
	{
		return true;
	}

	bool NanoZynthAudioProcessor::acceptsMidi() const
	{
#if JucePlugin_WantsMidiInput
		return true;
#else
		return false;
#endif
	}

	bool NanoZynthAudioProcessor::producesMidi() const
	{
#if JucePlugin_ProducesMidiOutput
		return true;
#else
		return false;
#endif
	}

	bool NanoZynthAudioProcessor::silenceInProducesSilenceOut() const
	{
		return false;
	}

	double NanoZynthAudioProcessor::getTailLengthSeconds() const
	{
		return 0.0;
	}

	int NanoZynthAudioProcessor::getNumPrograms()
	{
		return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
					// so this should be at least 1, even if you're not really implementing programs.
	}

	int NanoZynthAudioProcessor::getCurrentProgram()
	{
		return 0;
	}

	void NanoZynthAudioProcessor::setCurrentProgram(int index)
	{
	}

	const String NanoZynthAudioProcessor::getProgramName(int index)
	{
		return String();
	}

	void NanoZynthAudioProcessor::changeProgramName(int index, const String& newName)
	{
	}

	void NanoZynthAudioProcessor::releaseResources()
	{
		// When playback stops, you can use this as an opportunity to free up any
		// spare memory, etc.
	}



	//==============================================================================
	bool NanoZynthAudioProcessor::hasEditor() const
	{
		return true; // (change this to false if you choose to not supply an editor)
	}

	AudioProcessorEditor* NanoZynthAudioProcessor::createEditor()
	{
//		DBGM("In NanoZynthAudioProcessor::createEditor() ");
		return new NanoZynthAudioProcessorEditor(*this);
	}

#pragma endregion
	//==============================================================================
	// This creates new instances of the plugin..
	AudioProcessor* JUCE_CALLTYPE createPluginFilter()
	{
//		DBGM("In NanoZynthAudioProcessor::createPluginFilter() ");
		return new NanoZynthAudioProcessor();
	}

