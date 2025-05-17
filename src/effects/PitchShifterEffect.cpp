#include "effects/PitchShifterEffect.hpp"
#include <algorithm>
#include <cmath>

PitchShifterEffect::PitchShifterEffect(float sampleRate)
    : semitones(0.0f), sampleRate(sampleRate), windowSize(1024), hopSize(256) {
    windowBuffer.resize(windowSize, 0.0f);
}

void PitchShifterEffect::setSemitones(float s) {
    semitones = std::clamp(s, -12.0f, 12.0f);
}

void PitchShifterEffect::timeStretch(const float* in, float* out, int inLen, float stretch) {
    // Simple WSOLA-like time stretching (not phase vocoder)
    int outLen = static_cast<int>(inLen * stretch);
    for (int i = 0; i < outLen; ++i) {
        int idx = static_cast<int>(i / stretch);
        if (idx < inLen) out[i] = in[idx];
        else out[i] = 0.0f;
    }
}

void PitchShifterEffect::process(float* input, float* output, std::size_t frameCount, int numChannels) {
    // Only process left channel (mono) for simplicity, copy right channel
    float pitchFactor = std::pow(2.0f, semitones / 12.0f);
    int stretchedLen = static_cast<int>(frameCount / pitchFactor);
    std::vector<float> stretched(stretchedLen, 0.0f);
    // Only process left channel
    std::vector<float> leftIn(frameCount);
    for (std::size_t n = 0; n < frameCount; ++n) leftIn[n] = input[n * numChannels];
    timeStretch(leftIn.data(), stretched.data(), (int)frameCount, 1.0f / pitchFactor);
    // Resample to output
    for (std::size_t n = 0; n < frameCount; ++n) {
        int idx = static_cast<int>(n / pitchFactor);
        float val = (idx < stretchedLen) ? stretched[idx] : 0.0f;
        output[n * numChannels] = val;
        // Copy right channel unchanged (we could apply same effect)
        if (numChannels > 1) output[n * numChannels + 1] = input[n * numChannels + 1];
    }
}
