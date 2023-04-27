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
  
  void ImGui_layer::on_ui_render(float time_step)
  {
    ImGuiIO &io = ImGui::GetIO();
    auto bold = io.Fonts->Fonts[1];
    io.DeltaTime = time_step;
    // Note: Switch this to true to enable dockspace
    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    
    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    
    if (opt_fullscreen)
    {
      ImGuiViewport *viewport_ = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport_->Pos);
      ImGui::SetNextWindowSize(viewport_->Size);
      ImGui::SetNextWindowViewport(viewport_->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                      ImGuiWindowFlags_NoMove;
      window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    
    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) window_flags |= ImGuiWindowFlags_NoBackground;
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Wave-Engine", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();  // Window Padding.
    ImGui::PopStyleVar();  // Window Border size.
    ImGui::PopStyleVar();  // Window Rounding.
    
    // DockSpace
    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowMinSize.x = 250.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
      ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
      ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    
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
          Engine::get_app()->shutdown();
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }
    
    if (ImGui::Begin("Scene"))
    {
      if (ImGui::TreeNodeEx("Clear color", ImGuiTreeNodeFlags_SpanFullWidth))
      {
        ImGui::PushFont(bold);
        ImGui::ColorEdit3("", &Engine::get_main_window()->get_bg_color()[0]);
        ImGui::PopFont();
        ImGui::TreePop();
      }
    }
    ImGui::End();  // Scene Hierarchy
    
    if (ImGui::Begin("Stats"))
    {
      ImGui::Text("Application performance :\t%.3f ms/frame (%d FPS)", 1000.0f * time_step,
                  static_cast<int>(Engine::get_engine_framerate()));
    }
    ImGui::End();  // Stats
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0,
                                                             0});
    ImGui::PopStyleVar();
    ImGui::End(); // Wave-Engine.
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