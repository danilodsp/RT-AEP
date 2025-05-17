#include <atomic>

#include "Visualizer.hpp"
// Global pointer for audio thread access
Visualizer* gVisualizer = nullptr;
#include <algorithm>
#include <cstring>

Visualizer::Visualizer(int sampleRate, int bufferMs)
    : sampleRate(sampleRate), writeIndex(0) {
    bufferSize = (sampleRate * bufferMs) / 1000;
    circularBuffer.resize(bufferSize, 0.0f);
}

void Visualizer::pushAudio(const float* data, int numSamples) {
    std::lock_guard<std::mutex> lock(mtx);
    for (int i = 0; i < numSamples; ++i) {
        circularBuffer[writeIndex] = data[i];
        writeIndex = (writeIndex + 1) % bufferSize;
    }
}

std::vector<float> Visualizer::getWaveform() const {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<float> out(bufferSize);
    int idx = writeIndex;
    for (int i = 0; i < bufferSize; ++i) {
        out[i] = circularBuffer[idx];
        idx = (idx + 1) % bufferSize;
    }
    return out;
}
