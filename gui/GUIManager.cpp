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
    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("RT-AEP Control Panel");
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
        ImGui::End();
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window_, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
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
