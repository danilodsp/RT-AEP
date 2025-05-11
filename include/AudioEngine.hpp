#pragma once
#include <portaudio.h>
#include <atomic>

// AudioEngine: Manages PortAudio stream for real-time audio passthrough

#include <memory>
#include "effects/EffectChain.hpp"

class AudioEngine {
public:
    AudioEngine();
    ~AudioEngine();
    bool initialize();
    bool start();
    bool stop();
private:
    PaStream* stream_;
    std::atomic<bool> running_;
    std::shared_ptr<EffectChain> effectChain_;
    static int paCallback(const void* input, void* output,
                         unsigned long frameCount,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void* userData);
    void printDiagnostics();
    void setupDefaultEffects();
};
