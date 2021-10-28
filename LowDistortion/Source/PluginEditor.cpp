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
    gainSlider.setRange(1.0f, 5.0f, 0.1f);
    addAndMakeVisible(gainSlider);
    gainSlider.addListener(this);
    gainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    //volSlider parameters
    volSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    volSlider.setRange(0.0f, 1.0f, 0.01f);
    volSlider.setValue(1.0f);
    addAndMakeVisible(volSlider);
    volSlider.addListener(this);
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
    g.drawText("Gain", getWidth() / 3 - 50, getHeight() / 2 + 5, 100, 100, juce::Justification::centred, false);
    g.drawText("Vol", (getWidth() / 3) * 2 - 50, getHeight() / 2 + 5, 100, 100, juce::Justification::centred, false);
}

void NewProjectAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    gainSlider.setBounds(getWidth() / 3 - 50, getHeight() / 2 - 50, 100, 100);
    volSlider.setBounds((getWidth() / 3) * 2 - 50, getHeight() / 2 - 50, 100, 100);
}

void NewProjectAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &gainSlider)
    {
        audioProcessor.gainVal = gainSlider.getValue();
    }

    if (slider == &volSlider)
    {
        audioProcessor.volVal = volSlider.getValue();
    }
}
