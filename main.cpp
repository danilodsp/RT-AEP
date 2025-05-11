
#include "AudioEngine.hpp"
#include <iostream>


#include "effects/GainEffect.hpp"
#include "effects/EchoEffect.hpp"
#include "effects/DistortionEffect.hpp"
#include <vector>
#include <cmath>

// Simple test for effect classes using a synthetic mono buffer
void testEffects() {
    constexpr std::size_t N = 32;
    float buffer[N] = {};
    // Fill with a ramp
    for (std::size_t i = 0; i < N; ++i) buffer[i] = (float)i / N;

    GainEffect gain(2.0f);
    gain.testProcessMono(buffer, N);
    std::cout << "After GainEffect (x2): ";
    for (std::size_t i = 0; i < 8; ++i) std::cout << buffer[i] << ' ';
    std::cout << "...\n";

    EchoEffect echo(4, 0.5f);
    echo.testProcessMono(buffer, N);
    std::cout << "After EchoEffect: ";
    for (std::size_t i = 0; i < 8; ++i) std::cout << buffer[i] << ' ';
    std::cout << "...\n";

    DistortionEffect dist(1.5f);
    dist.testProcessMono(buffer, N);
    std::cout << "After DistortionEffect: ";
    for (std::size_t i = 0; i < 8; ++i) std::cout << buffer[i] << ' ';
    std::cout << "...\n";
}


#include "gui/GUIManager.hpp"
#include <thread>

int main() {
    //std::cout << "Testing effects on synthetic buffer...\n";
    //testEffects();

    AudioEngine engine;
    if (!engine.initialize()) {
        std::cerr << "Failed to initialize AudioEngine." << std::endl;
        return 1;
    }
    if (!engine.start()) {
        std::cerr << "Failed to start AudioEngine." << std::endl;
        return 1;
    }

    // Start GUI
    GUIManager gui;
    if (!gui.initialize()) {
        std::cerr << "Failed to initialize GUI." << std::endl;
        engine.stop();
        return 1;
    }
    gui.renderLoop(engine.getEffectChain());
    gui.shutdown();

    engine.stop();
    return 0;
}
