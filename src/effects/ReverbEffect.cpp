#include "effects/ReverbEffect.hpp"
#include <algorithm>
#include <cstring>

// Example static delay lengths (in samples) for 44.1kHz
static constexpr int combDelays[ReverbEffect::numCombs] = {1116, 1188, 1277, 1356};
static constexpr int allPassDelays[ReverbEffect::numAllPasses] = {556, 441};

ReverbEffect::ReverbEffect(float sampleRate)
    : roomSize(0.5f), damping(0.5f), wet(0.3f), dry(0.7f), sampleRate(sampleRate) {
    combBuffers.resize(numCombs);
    combIndices.resize(numCombs, 0);
    combFeedback.resize(numCombs, 0.0f);
    for (int i = 0; i < numCombs; ++i) {
        combBuffers[i].resize(combDelays[i], 0.0f);
    }
    allPassBuffers.resize(numAllPasses);
    allPassIndices.resize(numAllPasses, 0);
    allPassFeedback.resize(numAllPasses, 0.0f);
    for (int i = 0; i < numAllPasses; ++i) {
        allPassBuffers[i].resize(allPassDelays[i], 0.0f);
    }
}

void ReverbEffect::setRoomSize(float size) { roomSize = std::clamp(size, 0.0f, 1.0f); }
void ReverbEffect::setDamping(float d) { damping = std::clamp(d, 0.0f, 1.0f); }
void ReverbEffect::setWet(float w) { wet = std::clamp(w, 0.0f, 1.0f); }
void ReverbEffect::setDry(float d) { dry = std::clamp(d, 0.0f, 1.0f); }

void ReverbEffect::resetBuffers() {
    for (auto& buf : combBuffers) std::fill(buf.begin(), buf.end(), 0.0f);
    for (auto& buf : allPassBuffers) std::fill(buf.begin(), buf.end(), 0.0f);
    std::fill(combIndices.begin(), combIndices.end(), 0);
    std::fill(allPassIndices.begin(), allPassIndices.end(), 0);
}

void ReverbEffect::process(float* input, float* output, std::size_t frameCount, int numChannels) {
    // Only process left channel (mono) for simplicity, copy right channel
    for (std::size_t n = 0; n < frameCount; ++n) {
        float inp = input[n * numChannels];
        float combSum = 0.0f;
        // Parallel comb filters
        for (int i = 0; i < numCombs; ++i) {
            float& buf = combBuffers[i][combIndices[i]];
            float y = buf;
            buf = inp + (buf * roomSize * (1.0f - damping));
            combIndices[i] = (combIndices[i] + 1) % combBuffers[i].size();
            combSum += y;
        }
        float ap = combSum;
        // Series all-pass filters
        for (int i = 0; i < numAllPasses; ++i) {
            float& buf = allPassBuffers[i][allPassIndices[i]];
            float y = buf;
            buf = ap + (y * 0.5f);
            ap = -ap + y;
            allPassIndices[i] = (allPassIndices[i] + 1) % allPassBuffers[i].size();
        }
        float wetOut = dry * inp + wet * ap;
        output[n * numChannels] = wetOut;
        // Copy right channel unchanged (we could apply same effect)
        if (numChannels > 1) output[n * numChannels + 1] = input[n * numChannels + 1];
    }
}
