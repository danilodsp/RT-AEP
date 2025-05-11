#include <cstddef>
#include "effects/GainEffect.hpp"
// Test utility: process a mono buffer in-place
void GainEffect::testProcessMono(float* buffer, std::size_t frameCount) {
    for (std::size_t i = 0; i < frameCount; ++i) {
        buffer[i] *= gain_;
    }
}
#include "effects/GainEffect.hpp"
#include <algorithm>

GainEffect::GainEffect(float gain) : gain_(gain) {}

void GainEffect::setGain(float gain) {
    gain_.store(gain);
}

float GainEffect::getGain() const {
    return gain_.load();
}

void GainEffect::process(float* input, float* output, std::size_t frameCount, int numChannels) {
    std::size_t total = frameCount * numChannels;
    float g = gain_.load();
    for (std::size_t i = 0; i < total; ++i) {
        output[i] = input[i] * g;
    }
}
