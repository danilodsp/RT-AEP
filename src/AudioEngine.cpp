
#include "../visualization/Visualizer.hpp"
extern Visualizer* gVisualizer;

#include "AudioEngine.hpp"
#include <memory>

std::shared_ptr<EffectChain> AudioEngine::getEffectChain() { return effectChain_; }

#include "AudioEngine.hpp"
#include <iostream>
#include <cstring>
#include <memory>
#include "effects/EffectChain.hpp"
#include "effects/GainEffect.hpp"
#include "effects/EchoEffect.hpp"
#include "effects/DistortionEffect.hpp"
#include "effects/ReverbEffect.hpp"
#include "effects/PitchShifterEffect.hpp"

// Constructor: initializes members
AudioEngine::AudioEngine() : stream_(nullptr), running_(false) {
    effectChain_ = std::make_shared<EffectChain>();
}

// Destructor: ensures stream is stopped and PortAudio is terminated
AudioEngine::~AudioEngine() {
    stop();
    Pa_Terminate();
}

// Initialize PortAudio and print diagnostics
bool AudioEngine::initialize() {
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }
    printDiagnostics();
    setupDefaultEffects();
    return true;
}

// Start the audio stream (duplex, passthrough)
bool AudioEngine::start() {
    if (running_) return true;
    PaStreamParameters inputParams, outputParams;
    std::memset(&inputParams, 0, sizeof(inputParams));
    std::memset(&outputParams, 0, sizeof(outputParams));
    inputParams.device = Pa_GetDefaultInputDevice();
    outputParams.device = Pa_GetDefaultOutputDevice();
    if (inputParams.device == paNoDevice || outputParams.device == paNoDevice) {
        std::cerr << "No default input/output device." << std::endl;
        return false;
    }
    inputParams.channelCount = 2;
    inputParams.sampleFormat = paFloat32;
    inputParams.suggestedLatency = Pa_GetDeviceInfo(inputParams.device)->defaultLowInputLatency;
    inputParams.hostApiSpecificStreamInfo = nullptr;
    outputParams.channelCount = 2;
    outputParams.sampleFormat = paFloat32;
    outputParams.suggestedLatency = Pa_GetDeviceInfo(outputParams.device)->defaultLowOutputLatency;
    outputParams.hostApiSpecificStreamInfo = nullptr;
    unsigned long framesPerBuffer = 256; // Low-latency target
    PaError err = Pa_OpenStream(&stream_, &inputParams, &outputParams, 44100, framesPerBuffer, paClipOff, paCallback, this);
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }
    err = Pa_StartStream(stream_);
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }
    running_ = true;
    return true;
}

// Stop the audio stream if running
bool AudioEngine::stop() {
    if (!running_) return true;
    if (stream_) {
        Pa_StopStream(stream_);
        Pa_CloseStream(stream_);
        stream_ = nullptr;
    }
    running_ = false;
    return true;
}

int AudioEngine::paCallback(const void* input, void* output,
                           unsigned long frameCount,
                           const PaStreamCallbackTimeInfo* /*timeInfo*/,
                           PaStreamCallbackFlags /*statusFlags*/,
                           void* userData) {
    AudioEngine* engine = static_cast<AudioEngine*>(userData);
    float* out = static_cast<float*>(output);
    const float* in = static_cast<const float*>(input);
    if (!out) return paContinue;
    if (!in) {
        std::memset(out, 0, frameCount * 2 * sizeof(float));
        return paContinue;
    }
    // Use effect chain
    if (engine && engine->effectChain_) {
        // Cast away const for input (safe: not modified)
        engine->effectChain_->process(const_cast<float*>(in), out, frameCount, 2);
        // Visualization: push audio to visualizer if available
        // (Assume a static/global visualizer pointer for now, to be improved)
        extern Visualizer* gVisualizer;
        if (gVisualizer) {
            gVisualizer->pushAudio(out, frameCount * 2); // stereo
        }
    } else {
        std::memcpy(out, in, frameCount * 2 * sizeof(float));
    }
    return paContinue;
}
// Setup default effects: Gain, Echo, Distortion
void AudioEngine::setupDefaultEffects() {
    if (!effectChain_) return;
    effectChain_->clearEffects();
    effectChain_->addEffect(std::make_shared<GainEffect>(1.0f));
    effectChain_->addEffect(std::make_shared<EchoEffect>(500, 0.4f));
    effectChain_->addEffect(std::make_shared<DistortionEffect>(1.5f));
    effectChain_->addEffect(std::make_shared<ReverbEffect>(44100.0f));
    effectChain_->addEffect(std::make_shared<PitchShifterEffect>(44100.0f));
}

// Print PortAudio version, device names, and latency
void AudioEngine::printDiagnostics() {
    std::cout << "PortAudio version: " << Pa_GetVersionText() << std::endl;
    int inDev = Pa_GetDefaultInputDevice();
    int outDev = Pa_GetDefaultOutputDevice();
    if (inDev != paNoDevice) {
        const PaDeviceInfo* info = Pa_GetDeviceInfo(inDev);
        std::cout << "Input Device: " << info->name << ", Latency: " << info->defaultLowInputLatency * 1000 << " ms" << std::endl;
    }
    if (outDev != paNoDevice) {
        const PaDeviceInfo* info = Pa_GetDeviceInfo(outDev);
        std::cout << "Output Device: " << info->name << ", Latency: " << info->defaultLowOutputLatency * 1000 << " ms" << std::endl;
    }
    std::cout << "Frames per buffer: 256" << std::endl;
}
