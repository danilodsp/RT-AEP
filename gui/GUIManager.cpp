#include "GUIManager.hpp"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include "effects/EffectChain.hpp"

#include "effects/GainEffect.hpp"
#include "effects/EchoEffect.hpp"
#include "effects/DistortionEffect.hpp"
#include "effects/ReverbEffect.hpp"
#include "effects/PitchShifterEffect.hpp"
#include "../visualization/Visualizer.hpp"

GUIManager::GUIManager() {}
GUIManager::~GUIManager() { shutdown(); }

bool GUIManager::initialize() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!\n";
        return false;
    }
    // GL 3.0+ for ImGui
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    window_ = glfwCreateWindow(600, 400, "RT-AEP Control Panel", nullptr, nullptr);
    if (!window_) {
        std::cerr << "Failed to create GLFW window!\n";
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1); // Enable vsync
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    // Customize ImGui style
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.GrabRounding = 6.0f;
    style.ScrollbarRounding = 6.0f;
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;
    // Example color customization
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.18f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.13f, 0.14f, 0.18f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.30f, 0.36f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    initialized_ = true;
    return true;
}

void GUIManager::renderLoop(std::shared_ptr<EffectChain> effectChain) {
    if (!initialized_ || !effectChain) return;
    // Try to get effect pointers for direct parameter access
    auto gain = effectChain->getEffectByType<GainEffect>();
    auto echo = effectChain->getEffectByType<EchoEffect>();
    auto dist = effectChain->getEffectByType<DistortionEffect>();
    auto reverb = effectChain->getEffectByType<ReverbEffect>();
    auto pitch = effectChain->getEffectByType<PitchShifterEffect>();

    // Visualization setup
    extern Visualizer* gVisualizer;
    static bool visualizerInit = false;
    if (!visualizerInit) {
        if (!gVisualizer) gVisualizer = new Visualizer(44100, 500);
        visualizerInit = true;
    }
    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Draw only the ImGui window
        ImGui::SetNextWindowPos(ImVec2(40, 40), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(520, 320), ImGuiCond_Once);
        ImGui::Begin("RT-AEP Control Panel", nullptr, ImGuiWindowFlags_NoCollapse);
        if (gain) {
            float g = gain->gain_.load();
            if (ImGui::SliderFloat("Gain", &g, 0.0f, 2.0f)) gain->gain_.store(g);
        }
        ImGui::Separator();
        if (echo) {
            int delay = echo->delaySamples_.load();
            float fb = echo->feedback_.load();
            if (ImGui::SliderInt("Delay (samples)", &delay, 100, 2000)) echo->delaySamples_.store(delay);
            if (ImGui::SliderFloat("Feedback", &fb, 0.0f, 0.9f)) echo->feedback_.store(fb);
        }
        ImGui::Separator();
        if (dist) {
            float d = dist->drive_.load();
            if (ImGui::SliderFloat("Drive", &d, 0.5f, 5.0f)) dist->drive_.store(d);
        }

        ImGui::Separator();
        if (reverb) {
            static float roomSize = 0.5f, damping = 0.5f, wet = 0.3f, dry = 0.7f;
            ImGui::Text("Reverb");
            if (ImGui::SliderFloat("Room Size", &roomSize, 0.0f, 1.0f)) reverb->setRoomSize(roomSize);
            if (ImGui::SliderFloat("Damping", &damping, 0.0f, 1.0f)) reverb->setDamping(damping);
            if (ImGui::SliderFloat("Wet", &wet, 0.0f, 1.0f)) reverb->setWet(wet);
            if (ImGui::SliderFloat("Dry", &dry, 0.0f, 1.0f)) reverb->setDry(dry);
        }
        ImGui::Separator();
        if (pitch) {
            static float semitones = 0.0f;
            ImGui::Text("Pitch Shifter");
            if (ImGui::SliderFloat("Semitones", &semitones, -12.0f, 12.0f)) pitch->setSemitones(semitones);
        }
        ImGui::Separator();
        // Visualization
        ImGui::Text("Waveform Visualization");
        static std::vector<float> waveform;
        waveform = gVisualizer ? gVisualizer->getWaveform() : std::vector<float>();
        if (!waveform.empty()) {
            ImGui::PlotLines("Waveform", waveform.data(), (int)waveform.size(), 0, nullptr, -1.0f, 1.0f, ImVec2(480, 80));
        }
        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window_, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGuiStyle& style = ImGui::GetStyle();
        glClearColor(style.Colors[ImGuiCol_WindowBg].x, style.Colors[ImGuiCol_WindowBg].y, style.Colors[ImGuiCol_WindowBg].z, style.Colors[ImGuiCol_WindowBg].w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window_);
    }
}

void GUIManager::shutdown() {
    if (!initialized_) return;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    if (window_) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
    glfwTerminate();
    initialized_ = false;
}
