#include "effects/EffectChain.hpp"
#include <cstring>
#include <algorithm>

EffectChain::EffectChain()
    : tempBufferA_(nullptr), tempBufferB_(nullptr), tempBufferSize_(0) {}

EffectChain::~EffectChain() {
    delete[] tempBufferA_;
    delete[] tempBufferB_;
}

void EffectChain::addEffect(std::shared_ptr<AudioEffect> effect) {
    effects_.push_back(effect);
}

void EffectChain::clearEffects() {
    effects_.clear();
}

void EffectChain::ensureTempBuffers(std::size_t frameCount, int numChannels) {
    std::size_t required = frameCount * numChannels;
    if (required > tempBufferSize_) {
        delete[] tempBufferA_;
        delete[] tempBufferB_;
        tempBufferA_ = new float[required];
        tempBufferB_ = new float[required];
        tempBufferSize_ = required;
    }
}

void EffectChain::process(float* input, float* output, std::size_t frameCount, int numChannels) {
    if (effects_.empty()) {
        // No effects: passthrough
        if (output != input)
            std::memcpy(output, input, frameCount * numChannels * sizeof(float));
        return;
    }
    ensureTempBuffers(frameCount, numChannels);
    float* inBuf = input;
    float* outBuf = tempBufferA_;
    for (size_t i = 0; i < effects_.size(); ++i) {
        if (i == effects_.size() - 1) {
            // Last effect writes to output
            outBuf = output;
        } else {
            outBuf = (i % 2 == 0) ? tempBufferA_ : tempBufferB_;
        }
        effects_[i]->process(inBuf, outBuf, frameCount, numChannels);
        inBuf = outBuf;
    }
}
