/*
  ============================================================================
    Lowpass
    Created: 24 Jun 2024 10:16:3
    Author:  Fergal Davi
    this was modified from Jan Wilczek's "Lowpass Highpass Filter" URL: https://github.com/JanWilczek/lowpass-highpass-filte
  ============================================================================
*/
// LowpassFilter.h
#pragma once
#include <vector>
#include <JuceHeader.h>

class LowpassFilter
{
public:
    void setCutoffFrequency(float cutoffFrequency);
    void setSamplingRate(float samplingRate);
    void processBlock(juce::dsp::AudioBlock<float>& block);

private:
    float cutoffFrequency;
    float samplingRate;
    std::vector<float> dnBuffer;
};

