//
// Created by nami on 12/04/23.
//


// IMGUI
#include <imGUI/imgui.h>
#include <imGUI/imgui_internal.h>
#include <imGUI/backends/imgui_impl_glfw.h>
#include <imGUI/backends/imgui_impl_opengl3.h>

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
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
    float fontSize = 18.0f;// *2.0f;
    io.FontDefault = io.Fonts->AddFontFromFileTTF("../Wave/res/Fonts/Comfortaa/Comfortaa-Regular.ttf", fontSize);
    io.Fonts->AddFontFromFileTTF("../Wave/res/Fonts/Comfortaa/Comfortaa-Bold.ttf", fontSize * 1.05f);
    io.FontGlobalScale = 0.8f;
    io.DisplaySize = ImVec2(static_cast<float>(Engine::get_main_window()->get_width()) / 3.0f,
                            static_cast<float>(Engine::get_main_window()->get_height()) / 3.0f);
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    
    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      style.WindowRounding = 0.0f;
      style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    
    set_colors();
    
    Window *window = Engine::get_main_window();
    auto glfw_window = static_cast<GLFWwindow *>(static_cast<GLFWwindow *>(window->get_native_window()));
    
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(glfw_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
  }
  
  void ImGui_layer::on_update(float time_step)
  {
  
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
    // Avoid propagating mouse and keyboard events if the imgui window is focused on.
    e.handled |= e.is_in_category(EVENT_CATEGORY_MOUSE) & io.WantCaptureMouse;
    e.handled |= e.is_in_category(EVENT_CATEGORY_KEYBOARD) & io.WantCaptureKeyboard;
  }
  
  void ImGui_layer::begin()
  {
    // To be called on each frame.
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
  }
  
  void ImGui_layer::end()
  {
    // To be called on each frame after everything has been passed into the pipeline.
    ImGuiIO io = ImGui::GetIO();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      GLFWwindow* backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }
  }
  
  void ImGui_layer::set_colors()
  {
    auto &colors = ImGui::GetStyle().Colors;
    
    auto dark_grey = ImVec4(0.2f,
                            0.2f,
                            0.2f,
                            1.0f);
    auto light_grey = ImVec4(0.4f,
                             0.4f,
                             0.4f,
                             1.0f);
    
    // Window Background.
    colors[ImGuiCol_WindowBg] = ImVec4 {0.15f,
                                        0.15f,
                                        0.15f,
                                        1.0f};
    
    // Title Background.
    colors[ImGuiCol_TitleBg] = dark_grey;
    colors[ImGuiCol_TitleBgActive] = dark_grey;
    colors[ImGuiCol_TitleBgCollapsed] = light_grey;
    
    // Headers
    colors[ImGuiCol_Header] = dark_grey;
    colors[ImGuiCol_HeaderHovered] = light_grey;
    colors[ImGuiCol_HeaderActive] = dark_grey;
    
    // Frame BG
    colors[ImGuiCol_FrameBg] = dark_grey;
    colors[ImGuiCol_FrameBgHovered] = light_grey;
    colors[ImGuiCol_FrameBgActive] = dark_grey;
    
    // Tabs
    colors[ImGuiCol_Tab] = dark_grey;
    colors[ImGuiCol_TabHovered] = light_grey;
    colors[ImGuiCol_TabActive] = dark_grey;
    colors[ImGuiCol_TabUnfocused] = light_grey;
    colors[ImGuiCol_TabUnfocusedActive] = dark_grey;
    
    // Text
//    colors[ImGuiCol_Text] = ImVec4 {};
    
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
  }
  
  [[maybe_unused]] uint32_t ImGui_layer::get_active_widget_id()
  {
    return GImGui->ActiveId;
  }
}