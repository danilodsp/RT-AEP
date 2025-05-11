
#pragma once
#include <atomic>
#include <memory>
#include "AudioEffect.hpp"

/**
 * @brief Simple gain effect. Multiplies each sample by gain_.
 */
class GainEffect : public AudioEffect {
public:
    explicit GainEffect(float gain = 1.0f);
    void setGain(float gain);
    float getGain() const;
    void process(float* input, float* output, std::size_t frameCount, int numChannels) override;

    // Test utility: process a buffer in-place (mono)
    void testProcessMono(float* buffer, std::size_t frameCount);
public:
    std::atomic<float> gain_;
private:
};
