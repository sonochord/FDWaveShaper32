//==============================================================================
// FDWaveShaper32 Project
//==============================================================================
/*
 * Built using the JUCE framework and developed in C++.
 *
 * Copyright © [2024] Fergal Davis. All Rights Reserved.
 */
//==============================================================================


#pragma once

#include <JuceHeader.h>
#include "Tilt-eq.h"
#include "ff_meters/ff_meters.h"
#include "SimpleCompressor.h"
#include "Clipper.h"
#include "Sigmoid.h"
#include "LowpassFilter.h"
#include <juce_dsp/juce_dsp.h>

//==============================================================================
class FDWaveShaper32AudioProcessor :    public juce::AudioProcessor,
                                        public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    FDWaveShaper32AudioProcessor();
    ~FDWaveShaper32AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getApvts() { return parameters; }

    // Function declarations
    juce::AudioProcessorValueTreeState parameters;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    void processBand(juce::dsp::AudioBlock<float>& band, float effectMix, float curveAmount);
    float waveshape(float input, float effectMix, float curveAmount);
    // FIR Lowpass Filter


    void setCompressorParameters(float threshold, float knee, float attack, float release, float ratio, float makeUpGain);
    void updateClippingType(float driveAmount);
    void setClippingEnabled(bool enabled); 

    //==============================================================================
    // Add Meters
    foleys::LevelMeterSource& getInputMeterSource() { return inputMeterSource; }
    foleys::LevelMeterSource& getOutputMeterSource() { return outputMeterSource; }
    
private:

    void parameterChanged(const juce::String& parameterID, float newValue) override {}
    
    // Gain parameters
    float inputGain = 1.0f;
    float outputGain = 1.0f;

    // Effect parameters
    float effectMix = 0.5f;
    float curveAmount = 1.0f;
    float driveAmount = 1.0f;
    float drywetMix = 1.0f;

    // Tilt EQ
    Tilteq tiltEQ;
    
    //Lowpass Filter
    LowpassFilter lowpassFilter;
    std::atomic<float>* softenParameter = nullptr;

    // Simple Compressor
    SimpleCompressor compressor;
    // Clipper
    pe::processor::Clipper<float> clipper; //Peak Eater Clipper
    
    // Oversampling with linear-phase FIR filter
    juce::dsp::Oversampling<float> oversampler;

    // Delay buffer for latency compensation
    juce::AudioBuffer<float> delayBuffer;
    int delayBufferPos = 0;
    int delaySamples = 0;
    
    // Level Meter Sources
    foleys::LevelMeterSource inputMeterSource;
    foleys::LevelMeterSource outputMeterSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FDWaveShaper32AudioProcessor)
};
