/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    //gainSlider parameters
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    gainSlider.setRange(0.0f, 1.0f, 0.01f);
    addAndMakeVisible(gainSlider);
    gainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    //toneSlider parameters
    toneSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    toneSlider.setRange(0.0f, 1.0f, 0.01f);
    addAndMakeVisible(toneSlider);
    toneSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    //volSlider parameters
    volSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    volSlider.setRange(0.0f, 1.0f, 0.01f);
    addAndMakeVisible(volSlider);
    volSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    setSize (500, 500);
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::white);
    g.drawText("Gain", getWidth() / 4 - 50, getHeight() / 2 + 5, 100, 100, juce::Justification::centred, false);
    g.drawText("Tone", (getWidth() / 4) * 2 - 50, getHeight() / 2 + 5, 100, 100, juce::Justification::centred, false);
    g.drawText("Vol", (getWidth() / 4) * 3 - 50, getHeight() / 2 + 5, 100, 100, juce::Justification::centred, false);
}

void NewProjectAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    gainSlider.setBounds(getWidth() / 4 - 50, getHeight() / 2 - 50, 100, 100);
    toneSlider.setBounds((getWidth() / 4) * 2 - 50, getHeight() / 2 - 50, 100, 100);
    volSlider.setBounds((getWidth() / 4) * 3 - 50, getHeight() / 2 - 50, 100, 100);
}
