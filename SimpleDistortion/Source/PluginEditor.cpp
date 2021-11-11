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
    gainSlider.setRange(1.0f, 20.0f, 0.1f);
    addAndMakeVisible(gainSlider);
    gainSlider.addListener(this);
    gainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    //volSlider
    volSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    volSlider.setRange(0.0f, 1.0f, 0.01f);
    volSlider.setValue(1.0f);
    addAndMakeVisible(volSlider);
    volSlider.addListener(this);
    volSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    //lowSlider
    lowSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    lowSlider.setRange(20.0f, 150.0f, 1.0f);
    lowSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    lowSlider.setValue(75.0f);
    addAndMakeVisible(lowSlider);
    lowSlider.addListener(this);

    //midSlider
    midSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    midSlider.setRange(120.0f, 2000.0f, 1.0f);
    midSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    midSlider.setValue(1000.0f);
    addAndMakeVisible(midSlider);
    midSlider.addListener(this);

    //highSlider
    highSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    highSlider.setRange(1700.0f, 20000.0f, 1.0f);
    highSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    highSlider.setValue(10000.0f);
    addAndMakeVisible(highSlider);
    highSlider.addListener(this);

    //distType combobox
    distType.addItem("Hard Clipping", 1);
    distType.addItem("Soft Clipping", 2);
    distType.addListener(this);
    distType.setSelectedId(1, juce::dontSendNotification);
    addAndMakeVisible(distType);

    //eqOn combobox
    eqOn.addItem("On", 1);
    eqOn.addItem("Off", 2);
    eqOn.addListener(this);
    eqOn.setSelectedId(1, juce::dontSendNotification);
    addAndMakeVisible(eqOn);

    //distOn combobox
    distOn.addItem("On", 1);
    distOn.addItem("Off", 2);
    distOn.addListener(this);
    distOn.setSelectedId(1, juce::dontSendNotification);
    addAndMakeVisible(distOn);

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
    g.drawText("Gain", 117, 255, 100, 100, juce::Justification::centred, false);
    g.drawText("Vol", 283, 255, 100, 100, juce::Justification::centred, false);

    //draw text for eq sliders 
    g.drawText("Low", 90, 325, 100, 100, juce::Justification::centred, false);
    g.drawText("Mid", 90, 360, 100, 100, juce::Justification::centred, false);
    g.drawText("High", 90, 395, 100, 100, juce::Justification::centred, false);
}

void SimpleDistortionAudioProcessorEditor::resized()
{
    //gain and vol
    gainSlider.setBounds(117, 200, 100, 100);
    volSlider.setBounds(283, 200, 100, 100);

    //drop menue
    distType.setBounds(150, 142, 200, 50);

    //filter sliders
    lowSlider.setBounds(150, 350, 200, 50 );
    midSlider.setBounds(150, 385, 200, 50);
    highSlider.setBounds(150, 420, 200, 50);

    //setbounds for eqOn and distOn comboboxes
    eqOn.setBounds(15, 365, 100, 50);
    distOn.setBounds(15, 142, 100, 50);
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
    if (slider == &lowSlider)
    {
        audioProcessor.lowFilterVal = lowSlider.getValue();
    }
    if (slider == &midSlider)
    {
        audioProcessor.midFilterVal = midSlider.getValue();
    }
    if (slider == &highSlider)
    {
        audioProcessor.highFilterVal = highSlider.getValue();
    }
}

void SimpleDistortionAudioProcessorEditor::comboBoxChanged(juce::ComboBox* box)
{
    if (box == &distType)
    {
        audioProcessor.selectedDist = distType.getSelectedId();
    }
}
