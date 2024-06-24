#include "PluginProcessor.h"
#include "PluginEditor.h"


FDWaveShaper32AudioProcessorEditor::FDWaveShaper32AudioProcessorEditor (FDWaveShaper32AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Load the background image
    auto backgroundImage = juce::ImageCache::getFromMemory(BinaryData::grey_png, BinaryData::grey_pngSize);
    
    if (!backgroundImage.isNull())
    {
        backgroundImageComponent.setImage(backgroundImage, juce::RectanglePlacement::stretchToFit);
        backgroundImageComponent.toBack();
        addAndMakeVisible(backgroundImageComponent);
    }
    else
    {
        jassert(!backgroundImage.isNull());
    }
    // Define pastel blue colors
    auto trackPink = juce::Colour::fromRGB(92,91,91);
    auto pastelDarkBlue = juce::Colour::fromRGB(100, 149, 237);
    auto pastelPurple = juce::Colour::fromRGB(180,255,255);
    auto pastelDarkPink = juce::Colour::fromRGB(191,49,201);
    
    // Configure sliders, labels, and attachments
    inputLevelSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    inputLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    inputLevelSlider.setRange(-24.0, 24.0);
    inputLevelSlider.setColour(juce::Slider::thumbColourId, pastelDarkPink); // Thumb color
    inputLevelSlider.setColour(juce::Slider::rotarySliderFillColourId, pastelPurple); // Filled part color
    inputLevelSlider.setColour(juce::Slider::rotarySliderOutlineColourId, trackPink); // Track color
    inputLevelSlider.setColour(juce::Slider::backgroundColourId, pastelDarkBlue); // Background color
    addAndMakeVisible(inputLevelSlider);
    inputLevelAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getApvts(), "inputGain", inputLevelSlider));

    outputLevelSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    outputLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    outputLevelSlider.setRange(-24.0, 24.0);
    outputLevelSlider.setColour(juce::Slider::thumbColourId, pastelDarkPink); // Thumb color
    outputLevelSlider.setColour(juce::Slider::rotarySliderFillColourId, pastelPurple); // Filled part color
    outputLevelSlider.setColour(juce::Slider::rotarySliderOutlineColourId, trackPink); // Track color
    outputLevelSlider.setColour(juce::Slider::backgroundColourId, pastelDarkBlue); // Background color
    addAndMakeVisible(outputLevelSlider);
    outputLevelAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getApvts(), "outputGain", outputLevelSlider));

    tiltFreqSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    tiltFreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    tiltFreqSlider.setRange(100.0, 2000.0);
    tiltFreqSlider.setColour(juce::Slider::thumbColourId, pastelDarkPink); // Thumb color
    tiltFreqSlider.setColour(juce::Slider::rotarySliderFillColourId, pastelPurple); // Filled part color
    tiltFreqSlider.setColour(juce::Slider::rotarySliderOutlineColourId, trackPink); // Track color
    tiltFreqSlider.setColour(juce::Slider::backgroundColourId, pastelDarkBlue); // Background color
    addAndMakeVisible(tiltFreqSlider);
    tiltFreqAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getApvts(), "tiltFreq", tiltFreqSlider));

    tiltGainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    tiltGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    tiltGainSlider.setRange(-6.0, 6.0);
    tiltGainSlider.setColour(juce::Slider::thumbColourId, pastelDarkPink); // Thumb color
    tiltGainSlider.setColour(juce::Slider::rotarySliderFillColourId, pastelPurple); // Filled part color
    tiltGainSlider.setColour(juce::Slider::rotarySliderOutlineColourId, trackPink); // Track color
    tiltGainSlider.setColour(juce::Slider::backgroundColourId, pastelDarkBlue); // Background color
    addAndMakeVisible(tiltGainSlider);
    tiltGainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getApvts(), "tiltGain", tiltGainSlider));
    
    softenSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    softenSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    softenSlider.setRange(0.0, 1.0);
    softenSlider.setColour(juce::Slider::thumbColourId, pastelDarkPink); // Thumb color
    softenSlider.setColour(juce::Slider::rotarySliderFillColourId, pastelPurple); // Filled part color
    softenSlider.setColour(juce::Slider::rotarySliderOutlineColourId, trackPink); // Track color
    softenSlider.setColour(juce::Slider::backgroundColourId, pastelDarkBlue); // Background color
    addAndMakeVisible(softenSlider);
    softenAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getApvts(), "soften", softenSlider));
    
    
    effectSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    effectSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    effectSlider.setRange(0.0, 1.0);
    effectSlider.setColour(juce::Slider::thumbColourId, pastelDarkPink); // Thumb color
    effectSlider.setColour(juce::Slider::rotarySliderFillColourId, pastelPurple); // Filled part color
    effectSlider.setColour(juce::Slider::rotarySliderOutlineColourId, trackPink); // Track color
    effectSlider.setColour(juce::Slider::backgroundColourId, pastelDarkBlue); // Background color
    addAndMakeVisible(effectSlider);
    effectAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getApvts(), "effectMix", effectSlider));

    curveSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    curveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    curveSlider.setRange(1.0f, 10.0f);
    curveSlider.setColour(juce::Slider::thumbColourId, pastelDarkPink); // Thumb color
    curveSlider.setColour(juce::Slider::rotarySliderFillColourId, pastelPurple); // Filled part color
    curveSlider.setColour(juce::Slider::rotarySliderOutlineColourId, trackPink); // Track color
    curveSlider.setColour(juce::Slider::backgroundColourId, pastelDarkBlue); // Background color
    addAndMakeVisible(curveSlider);
    curveAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getApvts(), "curveAmount", curveSlider));
    

    mixSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    mixSlider.setRange(0.0, 1.0);
    mixSlider.setColour(juce::Slider::thumbColourId, pastelDarkPink); // Thumb color
    mixSlider.setColour(juce::Slider::rotarySliderFillColourId, pastelPurple); // Filled part color
    mixSlider.setColour(juce::Slider::rotarySliderOutlineColourId, trackPink); // Track color
    mixSlider.setColour(juce::Slider::backgroundColourId, pastelDarkBlue); // Background color
    addAndMakeVisible(mixSlider);
    mixAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getApvts(), "drywetMix", mixSlider));

    // Configure labels
    inputLevelLabel.setText("Input", juce::dontSendNotification);
    inputLevelLabel.attachToComponent(&inputLevelSlider, false);
    inputLevelLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(inputLevelLabel);

    outputLevelLabel.setText("Output", juce::dontSendNotification);
    outputLevelLabel.attachToComponent(&outputLevelSlider, false);
    outputLevelLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(outputLevelLabel);

    tiltFreqLabel.setText("Fulcrum", juce::dontSendNotification);
    tiltFreqLabel.attachToComponent(&tiltFreqSlider, false);
    tiltFreqLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(tiltFreqLabel);

    tiltGainLabel.setText("Dark/Light", juce::dontSendNotification);
    tiltGainLabel.attachToComponent(&tiltGainSlider, false);
    tiltGainLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(tiltGainLabel);
    
    softenLabel.setText("Shade", juce::dontSendNotification);
    softenLabel.attachToComponent(&softenSlider, false);
    softenLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(softenLabel);

    effectLabel.setText("Effect", juce::dontSendNotification);
    effectLabel.attachToComponent(&effectSlider, false);
    effectLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(effectLabel);

    curveLabel.setText("Curve", juce::dontSendNotification);
    curveLabel.attachToComponent(&curveSlider, false);
    curveLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(curveLabel);
    
    // Custom colour for the Clipper label
    clipperEnabledLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    clipperEnabledLabel.setText("Clip", juce::dontSendNotification);
    clipperEnabledLabel.attachToComponent(&clipperEnabledButton, false);
    clipperEnabledLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(clipperEnabledLabel);
    
    mixLabel.setText("Dry/Wet", juce::dontSendNotification);
    mixLabel.attachToComponent(&mixSlider, false);
    mixLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mixLabel);
    
    //buttons section label
    flatLabel.setText("Lift", juce::dontSendNotification);
    flatLabel.attachToComponent(&flatButton, false);
    flatLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(flatLabel);

    addAndMakeVisible(inputMeter);
    addAndMakeVisible(outputMeter);
    

    // Using pointer notation
    inputMeter.setMeterSource(&audioProcessor.getInputMeterSource());
    outputMeter.setMeterSource(&audioProcessor.getOutputMeterSource());

    inputMeter.setLookAndFeel(&lnf);
    outputMeter.setLookAndFeel(&lnf);

    // Add compressor setting buttons
    flatButton.addListener(this);
    setting1Button.addListener(this);
    setting2Button.addListener(this);
    addAndMakeVisible(flatButton);
    addAndMakeVisible(setting1Button);
    addAndMakeVisible(setting2Button);
    
    // Add Clipper Button
    clipperEnabledButton.addListener(this);
    addAndMakeVisible(clipperEnabledButton);
    

    // Set the look and feel for the buttons
    flatButton.setLookAndFeel(&lookAndFeel);
    setting1Button.setLookAndFeel(&lookAndFeel);
    setting2Button.setLookAndFeel(&lookAndFeel);
    clipperEnabledButton.setLookAndFeel(&lookAndFeel);
    
    // Set buttons to toggle (on/off)
    flatButton.setClickingTogglesState(true);
    setting1Button.setClickingTogglesState(true);
    setting2Button.setClickingTogglesState(true);
    clipperEnabledButton.setClickingTogglesState(true);

    flatButton.addListener(this);
    setting1Button.addListener(this);
    setting2Button.addListener(this);
    clipperEnabledButton.addListener(this);
    
    // Set the flatButton to be enabled by default
    flatButton.setToggleState(true, juce::dontSendNotification);
 
    // Set the custom colour for the buttons
    flatButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour::fromRGB(251,166,170));
    flatButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(211, 198, 224));
    setting1Button.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(211, 198, 224));
    setting1Button.setColour(juce::TextButton::buttonOnColourId, juce::Colour::fromRGB(236,132,187));
    setting2Button.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(211, 198, 224));
    setting2Button.setColour(juce::TextButton::buttonOnColourId, juce::Colour::fromRGB(191,49,201));
    clipperEnabledButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour::fromRGB(255, 26, 111)); 
    clipperEnabledButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(211, 198, 224));

    
    setSize(500, 350); // Set plugin size here
}



FDWaveShaper32AudioProcessorEditor::~FDWaveShaper32AudioProcessorEditor()
{
    inputMeter.setLookAndFeel(nullptr);
    outputMeter.setLookAndFeel(nullptr);
    flatButton.setLookAndFeel(nullptr);
    setting1Button.setLookAndFeel(nullptr);
    setting2Button.setLookAndFeel(nullptr);
}

void FDWaveShaper32AudioProcessorEditor::paint (juce::Graphics& g)
{
    // Background
    backgroundImageComponent.setBounds(getLocalBounds());
    
}

void FDWaveShaper32AudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();

    // Retain the logo and meters area
    auto logoArea = bounds.removeFromTop(80).removeFromLeft(150);
    logoImageComponent.setBounds(logoArea);

    auto metersArea = bounds.removeFromTop(40);
    inputMeter.setBounds(metersArea.removeFromLeft(metersArea.getWidth() / 2).reduced(5));
    outputMeter.setBounds(metersArea.reduced(5));

    // Define common dimensions and spacing
    auto sliderSize = 60;
    auto driveSliderSize = 60; // Adjusted size for the Drive slider
    auto labelOffset = 15;
    auto sliderYPositionTopRow = metersArea.getBottom() + 20;  // Y position for the top row of sliders
    auto sliderYPositionBottomRow = sliderYPositionTopRow + sliderSize + 30;  // Y position for the bottom row of sliders
    auto sliderYPositionThirdRow = sliderYPositionBottomRow + sliderSize + 15;  // Y position for the third row of sliders (adjusted to move up)
    auto sectionWidth = bounds.getWidth() / 4;  // Each section takes a quarter of the width

    // Layout sliders and labels in four columns according to the new plan
    // Top Row
    inputLevelSlider.setBounds(sectionWidth * 0 + (sectionWidth - sliderSize) / 2, sliderYPositionTopRow, sliderSize, sliderSize);
    tiltFreqSlider.setBounds(sectionWidth * 1 + (sectionWidth - sliderSize) / 2, sliderYPositionTopRow, sliderSize, sliderSize);
    effectSlider.setBounds(sectionWidth * 2 + (sectionWidth - sliderSize) / 2, sliderYPositionTopRow, sliderSize, sliderSize);
    outputLevelSlider.setBounds(sectionWidth * 3 + (sectionWidth - sliderSize) / 2, sliderYPositionTopRow, sliderSize, sliderSize);
    // Second Row
    tiltGainSlider.setBounds(sectionWidth * 1 + (sectionWidth - sliderSize) / 2, sliderYPositionBottomRow - 12, sliderSize, sliderSize);
    curveSlider.setBounds(sectionWidth * 2 + (sectionWidth - sliderSize) / 2, sliderYPositionBottomRow - 12, sliderSize, sliderSize);
    mixSlider.setBounds(sectionWidth * 3 + (sectionWidth - sliderSize) / 2, sliderYPositionBottomRow - 12, sliderSize, sliderSize);
    // Third Row
    softenSlider.setBounds(sectionWidth * 1 + (sectionWidth - sliderSize) / 2, sliderYPositionThirdRow - 15 , sliderSize, sliderSize);
    clipperEnabledButton.setBounds(sectionWidth * 2 + (sectionWidth - 40) / 2, sliderYPositionThirdRow + 10, driveSliderSize, driveSliderSize);

    inputLevelLabel.setBounds(inputLevelSlider.getX(), inputLevelSlider.getY() - labelOffset, sliderSize, labelOffset);
    tiltFreqLabel.setBounds(tiltFreqSlider.getX(), tiltFreqSlider.getY() - labelOffset, sliderSize, labelOffset);
    tiltGainLabel.setBounds(tiltGainSlider.getX(), tiltGainSlider.getY() - labelOffset, sliderSize, labelOffset);
    softenLabel.setBounds(softenSlider.getX(), softenSlider.getY() - 10, sliderSize, labelOffset);
    effectLabel.setBounds(effectSlider.getX(), effectSlider.getY() - labelOffset, sliderSize, labelOffset);
    curveLabel.setBounds(curveSlider.getX(), curveSlider.getY() - labelOffset, sliderSize, labelOffset);
    clipperEnabledButton.setBounds(clipperEnabledButton.getX(), clipperEnabledButton.getY(), 40, labelOffset); // Adjust label position relative to the slider
    outputLevelLabel.setBounds(outputLevelSlider.getX(), outputLevelSlider.getY() - labelOffset, sliderSize, labelOffset);
    mixLabel.setBounds(mixSlider.getX(), mixSlider.getY() - labelOffset, sliderSize, labelOffset);
    flatLabel.setBounds(flatButton.getX(), flatButton.getY() - labelOffset, 50, labelOffset);

    // Layout buttons vertically below the Input slider and move them up closer
    auto buttonWidth = 35;
    auto buttonHeight = 15;
    auto buttonXPosition = sectionWidth * 0 + (sectionWidth - buttonWidth) / 2; // Center within the first column
    auto buttonYOffset = sliderYPositionBottomRow - 30; // Adjust this value to move the buttons up

    flatButton.setBounds(buttonXPosition, buttonYOffset + 20, buttonWidth, buttonHeight);
    setting1Button.setBounds(buttonXPosition, buttonYOffset + 50, buttonWidth, buttonHeight);
    setting2Button.setBounds(buttonXPosition, buttonYOffset + 80, buttonWidth, buttonHeight);
}

void FDWaveShaper32AudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &flatButton)
    {
        audioProcessor.setCompressorParameters(0.0f, 0.0f, 100.0f, 0.0f, 1.0f, 0.0f);
        setting1Button.setToggleState(false, juce::dontSendNotification);
        setting2Button.setToggleState(false, juce::dontSendNotification);
    }
    else if (button == &setting1Button)
    {
        audioProcessor.setCompressorParameters(-40.0f, 20.0f, 40.0f, 0.0f, 1.0f, 4.1f);
        flatButton.setToggleState(false, juce::dontSendNotification);
        setting2Button.setToggleState(false, juce::dontSendNotification);
    }
    else if (button == &setting2Button)
    {
        audioProcessor.setCompressorParameters(-40.0f, 30.0f, 100.0f, 800.0f, 10.0f, 6.2f);
        flatButton.setToggleState(false, juce::dontSendNotification);
        setting1Button.setToggleState(false, juce::dontSendNotification);
    }
    else if (button == &clipperEnabledButton)
    {
        bool isEnabled = clipperEnabledButton.getToggleState();
        audioProcessor.setClippingEnabled(isEnabled);
    }
}
