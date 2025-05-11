#pragma once
#include "AudioEffect.hpp"
#include <vector>

/**
 * @brief Mono echo effect with feedback. Applies to channel 0 only.
 */
class EchoEffect : public AudioEffect {
public:
    EchoEffect(int delaySamples, float feedback);
    void setDelay(int delaySamples);
    void setFeedback(float feedback);
    void process(float* input, float* output, std::size_t frameCount, int numChannels) override;

    // Test utility: process a mono buffer in-place
    void testProcessMono(float* buffer, std::size_t frameCount);
private:
    std::vector<float> buffer_;
    int delaySamples_;
    float feedback_;
    std::size_t writeIndex_;
};
