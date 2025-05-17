#pragma once
#include <vector>
#include <mutex>

#ifdef _WIN32
#define VISUALIZER_API __declspec(dllexport)
#else
#define VISUALIZER_API
#endif

class Visualizer {
public:
    VISUALIZER_API Visualizer(int sampleRate, int bufferMs = 500);
    void pushAudio(const float* data, int numSamples);
    std::vector<float> getWaveform() const;
    // TODO: add getSpectrogram()
private:
    int sampleRate;
    int bufferSize;
    std::vector<float> circularBuffer;
    int writeIndex;
    mutable std::mutex mtx;
};
