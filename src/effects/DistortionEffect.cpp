#include <cstddef>
#include "effects/DistortionEffect.hpp"
// Test utility: process a mono buffer in-place
void DistortionEffect::testProcessMono(float* buffer, std::size_t frameCount) {
    for (std::size_t i = 0; i < frameCount; ++i) {
        float x = buffer[i] * drive_;
        if (x > 1.0f) x = 1.0f;
        else if (x < -1.0f) x = -1.0f;
        buffer[i] = x;
    }
}
#include "effects/DistortionEffect.hpp"
#include <algorithm>

DistortionEffect::DistortionEffect(float drive) : drive_(drive) {}

void DistortionEffect::setDrive(float drive) {
    drive_ = drive;
}

float DistortionEffect::getDrive() const {
    return drive_;
}

void DistortionEffect::process(float* input, float* output, std::size_t frameCount, int numChannels) {
    std::size_t total = frameCount * numChannels;
    for (std::size_t i = 0; i < total; ++i) {
        float x = input[i] * drive_;
        // Hard clip
        if (x > 1.0f) x = 1.0f;
        else if (x < -1.0f) x = -1.0f;
        output[i] = x;
    }
}
