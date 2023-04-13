//
// Created by nami on 12/04/23.
//


// IMGUI
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_internal.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>

// imguizmo (Thecherno).
//#include <imGUIzmo/ImGuizmo.h>

// Wave
#include <ImGUI/imGUI_layer.h>
#include <Core/engine.h>

namespace Wave
{
  
  
  ImGui_layer::ImGui_layer()
  {
    this->layer_name = "ImGUI Layer";
  }
  
  void ImGui_layer::on_attach()
  {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    #if defined(WAVE_PLATFORM_WINDOWS)
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
    #endif
    float fontSize = 16.0f;// *2.0f;
    io.Fonts->AddFontFromFileTTF("../Wave/Resources/Fonts/Comfortaa.ttf", fontSize);
    io.FontDefault = io.Fonts->AddFontFromFileTTF("../Wave/Resources/Fonts/Comfortaa.ttf", fontSize);
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    
    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    #if defined(WAVE_PLATFORM_WINDOWS)
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      style.WindowRounding = 0.0f;
      style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    #endif
    
    set_colors();
    
    Window *window = Engine::get_main_window();
    auto glfw_window = static_cast<GLFWwindow *>(static_cast<GLFWwindow *>(window->get_native_window()));
    
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(glfw_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
  }
  
  void ImGui_layer::on_update(float time_step)
  {
    ImGui_layer::begin();
    ImGui::Begin("Settings");
    
    ImGui::Text("Renderer3D Stats:");
    ImGui::Text("Draw Calls: %d", 1);
    
    ImGui::ColorEdit3("Clear color", Engine::get_main_window()->get_bg_color());
    
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f * time_step, Engine::get_engine_framerate());
    
    ImGui::End();
    ImGui_layer::end();
  }
  
  void ImGui_layer::on_detach()
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }
  
  void ImGui_layer::on_event(Event &e)
  {
    ImGuiIO &io = ImGui::GetIO();
    e.handled |= e.is_in_category(EVENT_CATEGORY_MOUSE) & io.WantCaptureMouse;
//    e.handled |= e.is_in_category(EVENT_CATEGORY_KEYBOARD) & io.WantCaptureKeyboard;
  }
  
  void ImGui_layer::begin()
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
  }
  
  void ImGui_layer::end()
  {
    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    #if defined(WAVE_PLATFORM_WINDOWS)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      GLFWwindow* backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }
    #endif
  }
  
  void ImGui_layer::set_colors()
  {
    auto &colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4 {0.1f,
                                        0.105f,
                                        0.11f,
                                        1.0f};
    
    // Headers
    colors[ImGuiCol_Header] = ImVec4 {0.2f,
                                      0.205f,
                                      0.21f,
                                      1.0f};
    colors[ImGuiCol_HeaderHovered] = ImVec4 {0.3f,
                                             0.305f,
                                             0.31f,
                                             1.0f};
    colors[ImGuiCol_HeaderActive] = ImVec4 {0.15f,
                                            0.1505f,
                                            0.151f,
                                            1.0f};
    
    // Buttons
    colors[ImGuiCol_Button] = ImVec4 {0.2f,
                                      0.205f,
                                      0.21f,
                                      1.0f};
    colors[ImGuiCol_ButtonHovered] = ImVec4 {0.3f,
                                             0.305f,
                                             0.31f,
                                             1.0f};
    colors[ImGuiCol_ButtonActive] = ImVec4 {0.15f,
                                            0.1505f,
                                            0.151f,
                                            1.0f};
    
    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4 {0.2f,
                                       0.205f,
                                       0.21f,
                                       1.0f};
    colors[ImGuiCol_FrameBgHovered] = ImVec4 {0.3f,
                                              0.305f,
                                              0.31f,
                                              1.0f};
    colors[ImGuiCol_FrameBgActive] = ImVec4 {0.15f,
                                             0.1505f,
                                             0.151f,
                                             1.0f};
    
    // Tabs
    colors[ImGuiCol_Tab] = ImVec4 {0.15f,
                                   0.1505f,
                                   0.151f,
                                   1.0f};
    colors[ImGuiCol_TabHovered] = ImVec4 {0.38f,
                                          0.3805f,
                                          0.381f,
                                          1.0f};
    colors[ImGuiCol_TabActive] = ImVec4 {0.28f,
                                         0.2805f,
                                         0.281f,
                                         1.0f};
    colors[ImGuiCol_TabUnfocused] = ImVec4 {0.15f,
                                            0.1505f,
                                            0.151f,
                                            1.0f};
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4 {0.2f,
                                                  0.205f,
                                                  0.21f,
                                                  1.0f};
    
    // Title
    colors[ImGuiCol_TitleBg] = ImVec4 {0.15f,
                                       0.1505f,
                                       0.151f,
                                       1.0f};
    colors[ImGuiCol_TitleBgActive] = ImVec4 {0.15f,
                                             0.1505f,
                                             0.151f,
                                             1.0f};
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4 {0.15f,
                                                0.1505f,
                                                0.151f,
                                                1.0f};
  }
  
  [[maybe_unused]] uint32_t ImGui_layer::get_active_widget_id()
  {
    return GImGui->ActiveId;
  }
}