
#pragma once
#include <atomic>
#include <memory>
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
public:
    std::atomic<int> delaySamples_;
    std::atomic<float> feedback_;
private:
    std::vector<float> buffer_;
    std::size_t writeIndex_;
};
