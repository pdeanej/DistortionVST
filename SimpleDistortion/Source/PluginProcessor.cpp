/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleDistortionAudioProcessor::SimpleDistortionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), lowBandFilter(juce::dsp::IIR::Coefficients<float>::makeBandPass(44100, 150.0f))
                        , midBandFilter(juce::dsp::IIR::Coefficients<float>::makeBandPass(44100, 700.0f))
                        , highBandFilter(juce::dsp::IIR::Coefficients<float>::makeBandPass(44100, 2000.0f))
#endif
{
}

SimpleDistortionAudioProcessor::~SimpleDistortionAudioProcessor()
{
}

//==============================================================================
const juce::String SimpleDistortionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleDistortionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimpleDistortionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SimpleDistortionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SimpleDistortionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleDistortionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleDistortionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleDistortionAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SimpleDistortionAudioProcessor::getProgramName (int index)
{
    return {};
}

void SimpleDistortionAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SimpleDistortionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    lastSampleRate = sampleRate;
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    spec.sampleRate = sampleRate;

    lowBandFilter.prepare(spec);
    midBandFilter.prepare(spec);
    highBandFilter.prepare(spec);
    lowBandFilter.reset();
    midBandFilter.reset();
    highBandFilter.reset();
}

void SimpleDistortionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleDistortionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SimpleDistortionAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //EQ processing
    auto audioBlock = juce::dsp::AudioBlock<float>(buffer);
    updateFilter();
    lowBandFilter.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    midBandFilter.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    highBandFilter.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
           
            channelData[sample] = channelData[sample] * gainVal;
            
            //Hard cliping
            if (selectedDist == 1)
            {
                if (channelData[sample] > 0.2f)
                {
                    channelData[sample] = 0.2f;
                }
                if (channelData[sample] < -0.2f)
                {
                    channelData[sample] = -0.2f;
                }
            }

            //Soft Clipping
            if (selectedDist == 2)
            {
                if (channelData[sample] > 0.5f)
                {
                    channelData[sample] = 1.0f - expf(-channelData[sample]);
                }
                else if (channelData[sample] < - 0.5f)
                {
                    channelData[sample] = -1.0 + expf(channelData[sample]);
                }
                else
                {
                    channelData[sample] = channelData[sample];
                }
            }
            

            //vol control
            channelData[sample] = channelData[sample] * volVal;
        }
        
    }
}

//==============================================================================
bool SimpleDistortionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SimpleDistortionAudioProcessor::createEditor()
{
    return new SimpleDistortionAudioProcessorEditor (*this);
}

//==============================================================================
void SimpleDistortionAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SimpleDistortionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void SimpleDistortionAudioProcessor::updateFilter()
{

    *lowBandFilter.state = *juce::dsp::IIR::Coefficients<float>::makeBandPass(lastSampleRate, lowFilterVal);
    *midBandFilter.state = *juce::dsp::IIR::Coefficients<float>::makeBandPass(lastSampleRate, midFilterVal);
    *highBandFilter.state = *juce::dsp::IIR::Coefficients<float>::makeBandPass(lastSampleRate, highFilterVal);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleDistortionAudioProcessor();
}
