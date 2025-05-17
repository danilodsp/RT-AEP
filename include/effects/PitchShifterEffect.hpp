#pragma once
#include "AudioEffect.hpp"
#include <vector>

class PitchShifterEffect : public AudioEffect {
public:
    PitchShifterEffect(float sampleRate);
    void setSemitones(float semitones);
    void process(float* input, float* output, std::size_t frameCount, int numChannels) override;

private:
    float semitones;
    float sampleRate;
    std::vector<float> windowBuffer;
    int windowSize;
    int hopSize;
    void timeStretch(const float* in, float* out, int inLen, float stretch);
};
