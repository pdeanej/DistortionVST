/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SimpleDistortionAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener, public juce::ComboBox::Listener
{
public:
    SimpleDistortionAudioProcessorEditor (SimpleDistortionAudioProcessor&);
    ~SimpleDistortionAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;
    void comboBoxChanged(juce::ComboBox* box) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimpleDistortionAudioProcessor& audioProcessor;

    //volume and gain
    juce::Slider gainSlider;
    juce::Slider volSlider;

    //to select distorion type
    juce::ComboBox distType;
    //to turn distortion on/off
    juce::ComboBox distOn;
    //turn on/off EQ
    juce::ComboBox eqOn;

    //EQ
    juce::Slider lowSlider;
    juce::Slider midSlider;
    juce::Slider highSlider;




    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleDistortionAudioProcessorEditor)
};
