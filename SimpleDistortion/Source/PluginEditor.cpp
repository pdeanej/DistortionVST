/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleDistortionAudioProcessorEditor::SimpleDistortionAudioProcessorEditor (SimpleDistortionAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    //gainSlider
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    gainSlider.setRange(1.0f, 5.0f, 0.1f);
    addAndMakeVisible(gainSlider);
    gainSlider.addListener(this);
    gainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    //vol slider
    volSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    volSlider.setRange(0.0f, 1.0f, 0.01f);
    volSlider.setValue(1.0f);
    addAndMakeVisible(volSlider);
    volSlider.addListener(this);
    volSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    //distType combobox
    distType.addItem("Hard Clipping", 1);
    distType.addItem("Soft Clipping", 2);
    distType.addListener(this);
    distType.setSelectedId(1, juce::dontSendNotification);
    addAndMakeVisible(distType);

    setSize (500, 500);
}

SimpleDistortionAudioProcessorEditor::~SimpleDistortionAudioProcessorEditor()
{
}

//==============================================================================
void SimpleDistortionAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::blanchedalmond);
    g.drawText("Gain", getWidth() / 3 - 50, getHeight() / 2 + 5, 100, 100, juce::Justification::centred, false);
    g.drawText("Vol", (getWidth() / 3) * 2 - 50, getHeight() / 2 + 5, 100, 100, juce::Justification::centred, false);
}

void SimpleDistortionAudioProcessorEditor::resized()
{
    gainSlider.setBounds(getWidth() / 3 - 50, getHeight() / 2 - 50, 100, 100);
    volSlider.setBounds((getWidth() / 3) * 2 - 50, getHeight() / 2 - 50, 100, 100);
    distType.setBounds((getWidth() / 2) - 100, (getHeight() / 3) - 25, 200, 50);
}

void SimpleDistortionAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
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

void SimpleDistortionAudioProcessorEditor::comboBoxChanged(juce::ComboBox* box)
{
    if (box == &distType)
    {
        audioProcessor.selectedDist = distType.getSelectedId();
    }
}
