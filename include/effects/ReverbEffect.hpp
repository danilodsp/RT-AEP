#pragma once
#include "AudioEffect.hpp"
#include <vector>

class ReverbEffect : public AudioEffect {
public:
    ReverbEffect(float sampleRate);
    void setRoomSize(float size);
    void setDamping(float damping);
    void setWet(float wet);
    void setDry(float dry);
    void process(float* input, float* output, std::size_t frameCount, int numChannels) override;

private:
    float roomSize;
    float damping;
    float wet;
    float dry;
    float sampleRate;

    // Comb and all-pass filter buffers
public:
    static constexpr int numCombs = 4;
    static constexpr int numAllPasses = 2;
private:
    std::vector<std::vector<float>> combBuffers;
    std::vector<int> combIndices;
    std::vector<std::vector<float>> allPassBuffers;
    std::vector<int> allPassIndices;
    std::vector<float> combFeedback;
    std::vector<float> allPassFeedback;
    void resetBuffers();
};
