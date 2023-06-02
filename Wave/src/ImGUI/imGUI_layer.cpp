//
// Created by nami on 12/04/23.
//


// IMGUI
#include <imGUI/backends/imgui_impl_glfw.h>
#include <imGUI/backends/imgui_impl_opengl3.h>

// Wave
#include <Core/engine.h>

namespace Wave
{
  bool ImGui_layer::dockSpace_open = true;
  ImGuiWindowFlags ImGui_layer::window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  ImGuiID ImGui_layer::dockSpace_id = 0, ImGui_layer::viewport_panel_dock_id = 0, ImGui_layer::scene_panel_dock_id = 0,
    ImGui_layer::events_panel_dock_id = 0, ImGui_layer::stats_panel_dock_id = 0;
  
  ImGui_layer::ImGui_layer(ImGui_layer_data_s imgui_data)
  {
    this->layer_name = "UI Layer";
    this->imgui_data = imgui_data;
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
    float fontSize = this->imgui_data.font_size;// *2.0f;
    io.FontDefault = io.Fonts->AddFontFromFileTTF("../Wave/res/Fonts/Comfortaa/Comfortaa-Regular.ttf",
                                                  fontSize);
    io.Fonts->AddFontFromFileTTF("../Wave/res/Fonts/Comfortaa/Comfortaa-Bold.ttf", fontSize);
    io.FontGlobalScale = this->imgui_data.font_scale;
    io.DisplaySize = ImVec2(static_cast<float>(Engine::get_main_window()->get_width()),
                            static_cast<float>(Engine::get_main_window()->get_height()));
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    
    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle &style = ImGui::GetStyle();
    style.AntiAliasedLinesUseTex = true;
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
    ImGui_ImplOpenGL3_Init(Renderer::get_api_shader_version());
  }
  
  void ImGui_layer::on_update([[maybe_unused]] float time_step)
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
  
  void ImGui_layer::on_render()
  {
#ifndef RUNTIME
    ImGuiIO &io = ImGui::GetIO();
    auto bold = io.Fonts->Fonts[1];
    // Note: Switch this to true to enable dockspace
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    
    if (opt_fullscreen)
    {
      ImGuiViewport *viewport_ = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport_->Pos);
      ImGui::SetNextWindowSize(viewport_->Size);
      ImGui::SetNextWindowViewport(viewport_->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      ImGui_layer::window_flags |=
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove;
      ImGui_layer::window_flags |=
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDecoration;
    }
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Wave-Engine", &(ImGui_layer::dockSpace_open), ImGui_layer::window_flags);
    ImGui::PopStyleVar();  // Window Padding.
    
    // DockSpace
    
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable && !ImGui_layer::dockSpace_id)
    {
      ImGui_layer::dockSpace_id = ImGui::GetID("Wave-Engine-DockSpace");
      ImGui::DockBuilderRemoveNode(ImGui_layer::dockSpace_id); // Clear out existing layout
      ImGui::DockBuilderAddNode(ImGui_layer::dockSpace_id, ImGuiDockNodeFlags_DockSpace); // Add empty node
      ImGui::DockBuilderSetNodeSize(ImGui_layer::dockSpace_id, ImVec2(Engine::get_main_window()->get_width(),
                                                                      Engine::get_main_window()->get_height()));
      
      ImGuiID dock_main_id = ImGui_layer::dockSpace_id;
      ImGui_layer::scene_panel_dock_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.15f, nullptr,
                                                                     &dock_main_id);
      ImGui_layer::stats_panel_dock_id = ImGui::DockBuilderSplitNode(ImGui_layer::scene_panel_dock_id, ImGuiDir_Down,
                                                                     0.4f, nullptr,
                                                                     &(ImGui_layer::scene_panel_dock_id));
      ImGui_layer::events_panel_dock_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.15f, nullptr,
                                                                      &dock_main_id);
      
      ImGui::DockBuilderDockWindow("Viewport", dock_main_id);
      ImGui::DockBuilderDockWindow("Scene", ImGui_layer::scene_panel_dock_id);
      ImGui::DockBuilderDockWindow("System Info", ImGui_layer::events_panel_dock_id);
      ImGui::DockBuilderDockWindow("Entity Info", ImGui_layer::stats_panel_dock_id);
      ImGui::DockBuilderFinish(dock_main_id);
      ImGui_layer::viewport_panel_dock_id = dock_main_id;
    }
    
    dockspace_flags |= ImDrawListFlags_AntiAliasedLinesUseTex;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
      ImGui_layer::dockSpace_id = ImGui::GetID("Wave-Engine-DockSpace");
      ImGui::DockSpace(ImGui_layer::dockSpace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    
    ImGui::PushFont(bold);
    if (ImGui::BeginMenuBar())
    {
      if (ImGui::BeginMenu("File"))
      {
        ImGui::MenuItem("Open Project...", "Ctrl+O");
        ImGui::Separator();
        ImGui::MenuItem("New Scene", "Ctrl+N");
        ImGui::MenuItem("Save Scene", "Ctrl+S");
        ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S");
        ImGui::Separator();
        
        if (ImGui::MenuItem("Exit"))
        {
          Engine::shutdown();
        }
        ImGui::EndMenu();
      }
    }
    ImGui::EndMenuBar();
    ImGui::PopFont();
    
    ImGui::End();  // Wave Engine.
    ImGui::PopStyleVar();  // Window Border size.
    ImGui::PopStyleVar();  // Window Rounding.
#endif
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
      GLFWwindow *backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }
  }
  
  void ImGui_layer::set_colors()
  {
    auto &colors = ImGui::GetStyle().Colors;
    
    auto dark_grey = ImVec4(0.1f,
                            0.1f,
                            0.1f,
                            1.0f);
    auto light_grey = ImVec4(0.4f,
                             0.4f,
                             0.4f,
                             1.0f);
    
    // Window Background.
    colors[ImGuiCol_WindowBg] = ImVec4{0.2f,
                                       0.2f,
                                       0.2f,
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
    colors[ImGuiCol_Button] = ImVec4{0.2f,
                                     0.205f,
                                     0.21f,
                                     1.0f};
    colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f,
                                            0.305f,
                                            0.31f,
                                            1.0f};
    colors[ImGuiCol_ButtonActive] = ImVec4{0.15f,
                                           0.1505f,
                                           0.151f,
                                           1.0f};
  }
  
  [[maybe_unused]] uint32_t ImGui_layer::get_active_widget_id()
  {
    return GImGui->ActiveId;
  }
}