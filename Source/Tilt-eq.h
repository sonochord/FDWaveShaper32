/*
*   Tilt-eq Plugin
*
*   Made by Jacob Curtis
*   Using JUCE Framework
*   Tested on Windows 10 using Reaper in VST3 format
*
*/

#pragma once

#include <JuceHeader.h>

using namespace juce;

struct Parameters
{
    float freq, gain;
};

class Tilteq
{
public:
    void setParameters(const AudioProcessorValueTreeState& apvts)
    {
        if (auto* freqParam = apvts.getRawParameterValue("tiltFreq"))
            parameters.freq = freqParam->load();
        else
            parameters.freq = 1000.0f; // Default value or handle the error appropriately

        if (auto* gainParam = apvts.getRawParameterValue("tiltGain"))
            parameters.gain = gainParam->load();
        else
            parameters.gain = 0.0f; // Default value or handle the error appropriately
    }

    void prepare(double newSampleRate, int maxBlockSize)
    {
        sampleRate = newSampleRate;
        dsp::ProcessSpec spec;
        spec.sampleRate = newSampleRate;
        spec.maximumBlockSize = maxBlockSize;
        spec.numChannels = 2;
        filterChain.prepare(spec);
    }

    void process(dsp::AudioBlock<float>& audioBlock)
    {
        *filterChain.get<0>().state = *dsp::IIR::Coefficients<float>::makeLowShelf(
            sampleRate, parameters.freq, 0.4f, Decibels::decibelsToGain(parameters.gain * -1.0f));
        *filterChain.get<1>().state = *dsp::IIR::Coefficients<float>::makeHighShelf(
            sampleRate, parameters.freq, 0.4f, Decibels::decibelsToGain(parameters.gain));

        dsp::ProcessContextReplacing<float> filterContext(audioBlock);
        filterChain.process(filterContext);
    }

private:
    double sampleRate{ 0.0 };
    Parameters parameters;
    using StereoFilter = dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>>;
    dsp::ProcessorChain<StereoFilter, StereoFilter> filterChain;
};
