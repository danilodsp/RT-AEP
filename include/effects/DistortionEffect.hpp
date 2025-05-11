
#pragma once
#include <atomic>
#include <memory>
#include "AudioEffect.hpp"

/**
 * @brief Simple distortion effect using hard clipping.
 */
class DistortionEffect : public AudioEffect {
public:
    explicit DistortionEffect(float drive = 1.0f);
    void setDrive(float drive);
    float getDrive() const;
    void process(float* input, float* output, std::size_t frameCount, int numChannels) override;

    // Test utility: process a buffer in-place (mono)
    void testProcessMono(float* buffer, std::size_t frameCount);
public:
    std::atomic<float> drive_;
private:
};
