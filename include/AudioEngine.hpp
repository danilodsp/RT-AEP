#pragma once
#include <portaudio.h>
#include <atomic>

// AudioEngine: Manages PortAudio stream for real-time audio passthrough
class AudioEngine {
public:
    AudioEngine(); // Constructor
    ~AudioEngine(); // Destructor
    bool initialize(); // Initialize PortAudio and diagnostics
    bool start();      // Start audio stream
    bool stop();       // Stop audio stream
private:
    PaStream* stream_;                // PortAudio stream handle
    std::atomic<bool> running_;       // Thread-safe running flag
    static int paCallback(const void* input, void* output,
                         unsigned long frameCount,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void* userData); // Audio callback
    void printDiagnostics();           // Print device/latency info
};
