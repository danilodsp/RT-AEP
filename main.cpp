#include "AudioEngine.hpp"
#include <iostream>

// Entry point for RT-AEP: initializes and runs the audio engine
int main() {
    AudioEngine engine;
    if (!engine.initialize()) {
        std::cerr << "Failed to initialize AudioEngine." << std::endl;
        return 1;
    }
    if (!engine.start()) {
        std::cerr << "Failed to start AudioEngine." << std::endl;
        return 1;
    }
    std::cout << "Press Enter to stop and exit..." << std::endl;
    std::cin.get(); // user input to exit
    engine.stop();
    return 0;
}
