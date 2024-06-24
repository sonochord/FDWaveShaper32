//==============================================================================
// FDWaveShaper32 Project
//==============================================================================
/*
 * FDWaveShaper32
 *
 * Author: Fergal Davis
 * Date: [Insert date or year of creation/last major update]
 *
 * Description:
 * FDWaveShaper32  is an audio plugin that combines various signal processing
 * techniques to shape and enhance audio signals. It incorporates a waveshaper, Tilt-EQ,
 * filter, compressor, and clipper to provide a versatile tool for sound design
 * and audio production.
 *
 * Key Features:
 * - Oversampling on the effected path for high-quality processing
 * - Customizable waveshaping algorithm for harmonic enhancement
 * - Tilt EQ for tonal balance adjustment
 * - Lowpass filter with variable cutoff for softening high frequencies
 * - Compressor with multiple presets for dynamic control
 * - Clipper for final stage limiting/clipping
 *
 *
 * This project integrates several third-party libraries and concepts,
 * combining them in a unique way to create a powerful and flexible
 * audio processing tool. The core waveshaping algorithm and overall
 * architecture were designed and implemented by Fergal Davis.
 *
 * Built using the JUCE framework and developed in C++.
 *
 * Copyright © [2024] Fergal Davis. All Rights Reserved.
 */
//==============================================================================
// Third-Party Libraries and Acknowledgements
//==============================================================================
/*
 * This project uses the following third-party libraries and resources:
 *
 * 1. "Peak Eater" Clipper
 *    Author: Vladyslav Voinov
 *    URL: https://github.com/vvvar/PeakEater
 *
 *    The "Peak Eater" clipper is used for signal limiting and shaping.
 *
 * 2. Tilt-eq Plugin
 *    Author: Jacob Curtis
 *    URL: https://github.com/jcurtis4207/Juce-Plugins/tree/master/Tilt-eq
 *
 *    The Tilt-eq plugin, made using the JUCE Framework, has been incorporated
 *    into this project. Tested on Windows 10 using Reaper in VST3 format.
 *
 * 3. Lowpass Highpass Filter
 *    Author: Jan Wilczek
 *    URL: https://github.com/JanWilczek/lowpass-highpass-filter
 *
 *    The lowpass and highpass filter implementations are based on Jan Wilczek's work.
 *
 * 4. SimpleCompressor
 *    Author: Daniel Rudrich
 *    URL: https://github.com/DanielRudrich/SimpleCompressor
 *    Copyright (c) 2019 Daniel Rudrich
 *
 *    The compression functionality in this project is derived from the SimpleCompressor project.
 *
 * @brief Meter component for displaying RMS, MAX, and CLIP values
 *
 * 5. ff_meters: This meter implementation is based on the ff_meters module by Foleys Finest Audio.
 *
 * Original Author: Daniel Walz
 * Copyright: (c) 2017 - 2020 Foleys Finest Audio Ltd.
 * License: BSD V2 3-clause
 * Version: 0.9.1
 * Website: https://github.com/ffAudio/ff_meters
 *
 *
 * I appreciate the work of all these authors in creating and sharing their
 * valuable audio processing tools and knowledge. Their contributions have been
 * instrumental in the development of this project.
 */
//==============================================================================


#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>
#include <juce_dsp/juce_dsp.h>

//==============================================================================

FDWaveShaper32AudioProcessor::FDWaveShaper32AudioProcessor()
: parameters(*this, nullptr, juce::Identifier("FDWaveShaper32"),
                 { std::make_unique<juce::AudioParameterFloat>("inputGain", "Input Gain", -24.0f, 24.0f, 0.0f),
                   std::make_unique<juce::AudioParameterFloat>("outputGain", "Output Gain", -24.0f, 24.0f, 0.0f),
                   std::make_unique<juce::AudioParameterFloat>("tiltFreq", "Tilt Frequency", 500.0f, 2000.0f, 1000.0f),
                   std::make_unique<juce::AudioParameterFloat>("tiltGain", "Tilt Gain", -6.0f, 6.0f, 0.0f),
                   std::make_unique<juce::AudioParameterFloat>("soften", "Soften", 0.0f, 1.0f, 0.0f),
                   std::make_unique<juce::AudioParameterFloat>("effectMix", "Effect Mix", 0.0f, 1.0f, 0.5f),
                   std::make_unique<juce::AudioParameterBool>("clipperEnabled", "Clipper Enabled", false),
                   std::make_unique<juce::AudioParameterFloat>("curveAmount", "Curve Amount", 1.0f, 10.0f, 5.0f),
                   std::make_unique<juce::AudioParameterFloat>("drywetMix", "Mix", 0.0f, 1.0f, 0.5f)}),
      oversampler(2, 4, juce::dsp::Oversampling<float>::FilterType::filterHalfBandFIREquiripple) // 4x linear-phase oversampling
{
    oversampler.setUsingIntegerLatency(true); // Ensure integer latency
    softenParameter = parameters.getRawParameterValue("soften");
    parameters.addParameterListener("soften", this);
}

FDWaveShaper32AudioProcessor::~FDWaveShaper32AudioProcessor()
{
    parameters.removeParameterListener("soften", this);
}


void FDWaveShaper32AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    oversampler.initProcessing(samplesPerBlock);
    //double oversampledRate = sampleRate * oversampler.getOversamplingFactor();  // I was using this to try and calculate the oversampled rate for the filter
    spec.sampleRate *= oversampler.getOversamplingFactor();
    spec.maximumBlockSize *= oversampler.getOversamplingFactor();

    tiltEQ.prepare(spec.sampleRate, spec.maximumBlockSize);
    compressor.prepare(spec);
    
    // "Peak Eater" clipper initialisation
    clipper.prepare(spec);
    clipper.setClippingType(pe::processor::ClippingType::LOGARITHMIC); // Set to Logarithmic clipping as it sounds smooth::: I like LOGARITHMIC, HARD, **CUBIC ** , **ALGEBRAIC**
    // **Clipping types**
    //    LOGARITHMIC
    //    HARD
    //    QUINTIC
    //    CUBIC
    //    HYPERBOLIC_TAN
    //    ALGEBRAIC
    //    ARCTANGENT
    //    SIN
    //    LIMIT
    

    lowpassFilter.setSamplingRate(static_cast<float>(sampleRate * oversampler.getOversamplingFactor()));
    
    delaySamples = static_cast<int>(oversampler.getLatencyInSamples());
    delayBuffer.setSize(spec.numChannels, delaySamples);
    delayBuffer.clear();
    delayBufferPos = 0;

    inputMeterSource.resize(getTotalNumInputChannels(), sampleRate * 0.1 / samplesPerBlock);
    outputMeterSource.resize(getTotalNumOutputChannels(), sampleRate * 0.1 / samplesPerBlock);
}

void FDWaveShaper32AudioProcessor::releaseResources() {}

bool FDWaveShaper32AudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainInputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
}

// This version is closer to the inflator waveshaping setup.
// I had the Effect parameter just working as a linear "wet/dry" mix but this way is working to maintain percieved loudness across the curves.
// I'm applying a scaling to the level based on the curve parameter setting so as to make the effect level more even.
float FDWaveShaper32AudioProcessor::waveshape(float input, float effectMix, float curveAmount)
{
    float absInput = std::abs(input);
    float sign = (input < 0.0f) ? -1.0f : 1.0f;

    // Calculate coefficients based on curveAmount
    float A = 1.0f + (curveAmount - 1.0f) / 9.0f;
    float B = -(curveAmount - 1.0f) / 45.0f;
    float C = (curveAmount - 6.0f) / 90.0f;
    float D = 0.0625f - (curveAmount - 1.0f) / 360.0f + ((curveAmount - 1.0f) * (curveAmount - 1.0f)) / 8100.0f;

    // Process input using the calculated polynomial
    float processedOutput = sign * (A * absInput + B * absInput * absInput + C * absInput * absInput * absInput - D * (absInput * absInput - 2.0f * absInput * absInput * absInput + absInput * absInput * absInput * absInput));

    // Calculate gain compensation
    float gainCompensation = 1.0f + (2.0f - 1.0f) * (10.0f - curveAmount) / 9.0f; // Linear interpolation from 2 (6dB) to 1 (0dB)

    // Mix processed output with original input based on effectMix and apply gain compensation
    float output = effectMix * processedOutput * gainCompensation + (1.0f - effectMix) * input;

    return output;
}



void FDWaveShaper32AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Retrieve parameter values safely
    if (auto* param = parameters.getRawParameterValue("inputGain"))
        inputGain = *param;
    if (auto* param = parameters.getRawParameterValue("outputGain"))
        outputGain = *param;
    if (auto* param = parameters.getRawParameterValue("effectMix"))
        effectMix = *param;
    if (auto* param = parameters.getRawParameterValue("curveAmount"))
        curveAmount = *param;
    if (auto* param = parameters.getRawParameterValue("drywetMix"))
        drywetMix = *param;

    // Apply input gain
    buffer.applyGain(juce::Decibels::decibelsToGain(inputGain));

    inputMeterSource.measureBlock(buffer);
    
    // Prepare the wet buffer
    juce::AudioBuffer<float> wetBuffer;
    wetBuffer.makeCopyOf(buffer);

    // Oversampling the wet buffer
    juce::dsp::AudioBlock<float> wetBlock(wetBuffer);
    auto oversampledBlock = oversampler.processSamplesUp(wetBlock);
    
    
    tiltEQ.setParameters(parameters);
    tiltEQ.process(oversampledBlock);

    // Process with the compressor
    juce::dsp::AudioBlock<float> compressorBlock(oversampledBlock);
    compressor.process(compressorBlock);

    

    // Process the band
    processBand(oversampledBlock, effectMix, curveAmount);
    
    // Lowpass filter
    float softenValue = *parameters.getRawParameterValue("soften");
    float cutoffFrequency = juce::jmap(softenValue, 0.0f, 1.0f, 12000.0f, 400.0f);
    lowpassFilter.setCutoffFrequency(cutoffFrequency);
    lowpassFilter.processBlock(oversampledBlock);
    
    //Downsample
    oversampler.processSamplesDown(wetBlock);

    // Delay compensation for the dry signal and mixing
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* dryData = buffer.getWritePointer(channel);
        auto* wetData = wetBuffer.getWritePointer(channel);
        auto* delayData = delayBuffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // Circular buffer management for delay
            int readPos = (delayBufferPos + sample - delaySamples + delayBuffer.getNumSamples()) % delayBuffer.getNumSamples();
            float delayedDrySample = delayData[readPos];

            // Write current dry sample to delay buffer
            delayData[(delayBufferPos + sample) % delayBuffer.getNumSamples()] = dryData[sample];

            // Mix delayed dry signal with wet signal
            dryData[sample] = delayedDrySample * (1.0f - drywetMix) + wetData[sample] * drywetMix;
        }
    }

    // Update delayBufferPos for the next block
    delayBufferPos = (delayBufferPos + buffer.getNumSamples()) % delayBuffer.getNumSamples();

    // Apply output gain
    buffer.applyGain(juce::Decibels::decibelsToGain(outputGain));
    
    // Apply clipping to the output signal
    bool clipperEnabled = *parameters.getRawParameterValue("clipperEnabled") > 0.5f;
    float threshold = juce::Decibels::decibelsToGain(-1.0f);
    clipper.setThreshold(threshold); // Ensure threshold is in linear scale
    if (clipperEnabled)
    {
        juce::dsp::AudioBlock<float> outputBlock(buffer);
        juce::dsp::ProcessContextReplacing<float> clippingContext(outputBlock);
        clipper.process(clippingContext);
    }

    // Push data to the meter sources
    outputMeterSource.measureBlock(buffer);
}


void FDWaveShaper32AudioProcessor::processBand(juce::dsp::AudioBlock<float>& band, float effectMix, float curveAmount)
{
    for (size_t channel = 0; channel < band.getNumChannels(); ++channel)
    {
        auto* bandData = band.getChannelPointer(channel);
        for (size_t sample = 0; sample < band.getNumSamples(); ++sample)
        {
            float input = bandData[sample];
            bandData[sample] = waveshape(input, effectMix, curveAmount);
        }
    }
}



void FDWaveShaper32AudioProcessor::setCompressorParameters(float threshold, float knee, float attack, float release, float ratio, float makeUpGain)
{
    compressor.setThreshold(threshold);
    compressor.setKnee(knee);
    compressor.setAttackTime(attack);
    compressor.setReleaseTime(release);
    compressor.setRatio(ratio);
    compressor.setMakeUpGain(makeUpGain);
}

void FDWaveShaper32AudioProcessor::setClippingEnabled(bool enabled)
{
    if (auto* param = parameters.getRawParameterValue("clipperEnabled"))
        *param = enabled ? 1.0f : 0.0f;
}

bool FDWaveShaper32AudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* FDWaveShaper32AudioProcessor::createEditor()
{
    return new FDWaveShaper32AudioProcessorEditor(*this);
}

void FDWaveShaper32AudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, true);
    parameters.state.writeToStream(stream);
}

void FDWaveShaper32AudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    juce::ValueTree tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid())
    {
        parameters.state = tree;
    }
}


const juce::String FDWaveShaper32AudioProcessor::getName() 

const { return JucePlugin_Name; }

bool FDWaveShaper32AudioProcessor::acceptsMidi() const { return false; }

bool FDWaveShaper32AudioProcessor::producesMidi() const { return false; }

bool FDWaveShaper32AudioProcessor::isMidiEffect() const { return false; }

double FDWaveShaper32AudioProcessor::getTailLengthSeconds() const { return 0.0; }

int FDWaveShaper32AudioProcessor::getNumPrograms() { return 1; }

int FDWaveShaper32AudioProcessor::getCurrentProgram() { return 0; }

void FDWaveShaper32AudioProcessor::setCurrentProgram(int index) {}

const juce::String FDWaveShaper32AudioProcessor::getProgramName(int index) { return {}; }

void FDWaveShaper32AudioProcessor::changeProgramName(int index, const juce::String& newName) {}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new FDWaveShaper32AudioProcessor();
}
