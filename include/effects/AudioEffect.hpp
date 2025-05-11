#pragma once
#include <cstddef>

/**
 * @brief Abstract base class for audio effects.
 * All audio effects must inherit from this class and implement the process method.
 */
class AudioEffect {
public:
    virtual ~AudioEffect() = default;

    /**
     * @brief Process audio frames.
     * @param input Pointer to input buffer (interleaved).
     * @param output Pointer to output buffer (interleaved).
     * @param frameCount Number of frames to process.
     * @param numChannels Number of audio channels (2 for stereo).
     */
    virtual void process(float* input, float* output, std::size_t frameCount, int numChannels) = 0;
};
