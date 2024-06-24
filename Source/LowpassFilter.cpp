/*
  =============================================================================
    Lowpass.c
    Created: 24 Jun 2024 10:17:44
    Author:  Fergal Davi
    this was modified from Jan Wilczek's "Lowpass Highpass Filter" URL: https://github.com/JanWilczek/lowpass-highpass-filter
  ============================================================================
*/

// LowpassFilter.cpp
#include "LowpassFilter.h"
#include <cmath>

void LowpassFilter::setCutoffFrequency(float cutoffFrequency) {
    this->cutoffFrequency = cutoffFrequency;
}

void LowpassFilter::setSamplingRate(float samplingRate) {
    this->samplingRate = samplingRate;
}

void LowpassFilter::processBlock(juce::dsp::AudioBlock<float>& block) {
    constexpr auto PI = 3.14159265359f;
    dnBuffer.resize(block.getNumChannels(), 0.f);

    const auto tan = std::tan(PI * cutoffFrequency / samplingRate);
    const auto a1 = (tan - 1.f) / (tan + 1.f);

    for (size_t channel = 0; channel < block.getNumChannels(); ++channel) {
        auto* channelSamples = block.getChannelPointer(channel);
        for (size_t i = 0; i < block.getNumSamples(); ++i) {
            const auto inputSample = channelSamples[i];
            const auto allpassFilteredSample = a1 * inputSample + dnBuffer[channel];
            dnBuffer[channel] = inputSample - a1 * allpassFilteredSample;

            const auto filterOutput = 0.5f * (inputSample + allpassFilteredSample);

            channelSamples[i] = filterOutput;
        }
    }
}
