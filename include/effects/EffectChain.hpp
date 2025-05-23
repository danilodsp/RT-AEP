
#pragma once
#include <atomic>
#include <memory>
#include <vector>
#include "AudioEffect.hpp"

/**
 * @brief Manages a chain of audio effects and applies them sequentially.
 */
class EffectChain {
public:
    // Utility: get effect by type (returns nullptr if not found)
    template<typename T>
    std::shared_ptr<T> getEffectByType() {
        for (auto& e : effects_) {
            auto ptr = std::dynamic_pointer_cast<T>(e);
            if (ptr) return ptr;
        }
        return nullptr;
    }
    EffectChain();
    ~EffectChain();

    /**
     * @brief Add an effect to the chain.
     * @param effect Shared pointer to an AudioEffect instance.
     */
    void addEffect(std::shared_ptr<AudioEffect> effect);

    /**
     * @brief Remove all effects from the chain.
     */
    void clearEffects();

    /**
     * @brief Process audio through the effect chain.
     * @param input Pointer to input buffer (interleaved).
     * @param output Pointer to output buffer (interleaved).
     * @param frameCount Number of frames to process.
     * @param numChannels Number of audio channels.
     */
    void process(float* input, float* output, std::size_t frameCount, int numChannels);

private:
    std::vector<std::shared_ptr<AudioEffect>> effects_;
    float* tempBufferA_;
    float* tempBufferB_;
    std::size_t tempBufferSize_;
    void ensureTempBuffers(std::size_t frameCount, int numChannels);
};
