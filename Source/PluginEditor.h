#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ff_meters/ff_meters.h"


class FDWaveShaper32AudioProcessorEditor : public juce::AudioProcessorEditor,
                                           private juce::Button::Listener
{
public:
    explicit FDWaveShaper32AudioProcessorEditor (FDWaveShaper32AudioProcessor&);
    ~FDWaveShaper32AudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void buttonClicked(juce::Button* button) override; 

    FDWaveShaper32AudioProcessor& audioProcessor;

    juce::ImageComponent backgroundImageComponent;
    juce::ImageComponent logoImageComponent;

    juce::Slider inputLevelSlider;
    juce::Slider outputLevelSlider;
    juce::Slider tiltFreqSlider;
    juce::Slider tiltGainSlider;
    juce::Slider softenSlider;
    juce::Slider effectSlider;
    juce::Slider curveSlider;
    juce::Slider mixSlider;

    juce::Label inputLevelLabel;
    juce::Label outputLevelLabel;
    juce::Label tiltFreqLabel;
    juce::Label tiltGainLabel;
    juce::Label softenLabel;
    juce::Label effectLabel;
    juce::Label curveLabel;
    juce::Label clipperEnabledLabel;
    juce::Label mixLabel;
    juce::Label flatLabel;

    juce::Label sectionInputLabel;
    juce::Label sectionToneLabel;
    juce::Label sectionEffectLabel;
    juce::Label sectionOutputLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputLevelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputLevelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> tiltFreqAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> tiltGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> softenAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> effectAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> curveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    
    foleys::LevelMeterLookAndFeel lnf; // Look and feel for the meter
    foleys::LevelMeter inputMeter { foleys::LevelMeter::Minimal | foleys::LevelMeter::Horizontal }; // The input meter
    foleys::LevelMeter outputMeter { foleys::LevelMeter::Minimal | foleys::LevelMeter::Horizontal }; // The output meter
    
    //compression buttons
    juce::TextButton flatButton { "0" };
    juce::TextButton setting1Button { "1" };
    juce::TextButton setting2Button { "2" };
    
    //clipper button
    juce::TextButton clipperEnabledButton { "Clip"};
    
    juce::LookAndFeel_V4 lookAndFeel; // Use the built-in LookAndFeel_V4 in JUCE

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FDWaveShaper32AudioProcessorEditor)
};

