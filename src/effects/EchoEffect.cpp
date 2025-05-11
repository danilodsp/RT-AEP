#include <cstddef>
#include "effects/EchoEffect.hpp"
// Test utility: process a mono buffer in-place
void EchoEffect::testProcessMono(float* buffer, std::size_t frameCount) {
    for (std::size_t i = 0; i < frameCount; ++i) {
        float inSample = buffer[i];
        float delayed = buffer_[writeIndex_];
        float outSample = inSample + delayed;
        buffer[i] = outSample;
        buffer_[writeIndex_] = inSample + delayed * feedback_;
        writeIndex_ = (writeIndex_ + 1) % buffer_.size();
    }
}
#include "effects/EchoEffect.hpp"
#include <cstring>

EchoEffect::EchoEffect(int delaySamples, float feedback)
    : delaySamples_(delaySamples), feedback_(feedback), writeIndex_(0) {
    buffer_.resize(delaySamples_ > 1 ? delaySamples_ : 1, 0.0f);
}

void EchoEffect::setDelay(int delaySamples) {
    delaySamples_ = delaySamples > 1 ? delaySamples : 1;
    buffer_.assign(delaySamples_, 0.0f);
    writeIndex_ = 0;
}

void EchoEffect::setFeedback(float feedback) {
    feedback_ = feedback;
}

void EchoEffect::process(float* input, float* output, std::size_t frameCount, int numChannels) {
    // Copy input to output for all channels
    std::memcpy(output, input, frameCount * numChannels * sizeof(float));
    for (std::size_t i = 0; i < frameCount; ++i) {
        float inSample = input[i * numChannels]; // channel 0 only
        float delayed = buffer_[writeIndex_];
        float outSample = inSample + delayed;
        output[i * numChannels] = outSample;
        buffer_[writeIndex_] = inSample + delayed * feedback_;
        writeIndex_ = (writeIndex_ + 1) % buffer_.size();
    }
}
