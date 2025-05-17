#pragma once
#include <memory>
#include <atomic>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include "effects/EffectChain.hpp"
#include <memory>

// Forward declarations for new effects and visualization
class ReverbEffect;
class PitchShifterEffect;
class Visualizer;

// GUIManager: Manages Dear ImGui + GLFW + OpenGL GUI for RT-AEP
class GUIManager {
public:
    GUIManager();
    ~GUIManager();

    // Initialize GLFW, OpenGL, and ImGui
    bool initialize();
    // Main render loop. Returns when window is closed.
    void renderLoop(std::shared_ptr<EffectChain> effectChain);
    // Shutdown and cleanup
    void shutdown();

private:
    GLFWwindow* window_ = nullptr;
    bool initialized_ = false;
};
