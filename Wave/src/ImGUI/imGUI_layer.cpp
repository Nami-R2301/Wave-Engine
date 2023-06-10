//
// Created by nami on 12/04/23.
//


// IMGUI
#include <imGUI/backends/imgui_impl_glfw.h>
#include <imGUI/backends/imgui_impl_opengl3.h>

// Wave
#include <Core/engine.h>
#include <ImGUI/imGUI_layer.h>


namespace Wave
{
  bool ImGui_layer::dockSpace_open = true, ImGui_layer::show_scene_panel = true, ImGui_layer::show_system_panel = true;
  ImGuiWindowFlags ImGui_layer::window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  ImGuiID ImGui_layer::dockSpace_id = 0, ImGui_layer::viewport_panel_dock_id = 0, ImGui_layer::scene_panel_dock_id = 0,
    ImGui_layer::events_panel_dock_id = 0, ImGui_layer::info_panel_dock_id = 0;
  ImGui_layer::ImGui_layer_data_s ImGui_layer::imgui_data;
  
  ImGui_layer::ImGui_layer(ImGui_layer_data_s imgui_data_)
  {
    this->layer_name = "UI Layer";
    ImGui_layer::imgui_data = imgui_data_;
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
    float fontSize = ImGui_layer::imgui_data.font_size;// *2.0f;
    io.FontDefault = io.Fonts->AddFontFromFileTTF("../Wave/res/Fonts/Roboto/Roboto-Regular.ttf",
                                                  fontSize);
    io.Fonts->AddFontFromFileTTF("../Wave/res/Fonts/Roboto/Roboto-Bold.ttf", fontSize);
    io.FontGlobalScale = ImGui_layer::imgui_data.font_scale;
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
  
  void ImGui_layer::on_update(float time_step)
  {
    ImGuiIO &io = ImGui::GetIO();
    io.DeltaTime = time_step > (1.0f / 60.0f) ? (1.0f / 60.0f) : time_step;
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
    
    // Attempting to zoom.
    if (e.get_event_type() == Event_type::On_mouse_wheel_scroll && Input::is_key_held(WAVE_KEY_LEFT_CONTROL))
    {
      auto on_mouse_zoom = dynamic_cast<On_mouse_wheel_scroll &>(e);
      ImGui_layer::imgui_data.font_scale += on_mouse_zoom.get_mouse_wheel_offset().get_y() * 0.0625f;
      io.FontGlobalScale = ImGui_layer::imgui_data.font_scale;
    }
    // Avoid propagating mouse and keyboard events if the imgui window is focused on.
    e.handled |= e.is_in_category(EVENT_CATEGORY_MOUSE) & io.WantCaptureMouse;
    e.handled |= e.is_in_category(EVENT_CATEGORY_KEYBOARD) & io.WantCaptureKeyboard;
  }
  
  void ImGui_layer::on_render()
  {
  }
  
  void ImGui_layer::on_ui_render(float time_step)
  {
#ifndef RUNTIME
    ImGuiIO &io = ImGui::GetIO();
    io.DeltaTime = time_step;
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
      ImGui_layer::scene_panel_dock_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, nullptr,
                                                                     &dock_main_id);
      ImGui_layer::info_panel_dock_id = ImGui::DockBuilderSplitNode(ImGui_layer::scene_panel_dock_id, ImGuiDir_Down,
                                                                    0.4f, nullptr,
                                                                    &(ImGui_layer::scene_panel_dock_id));
      ImGui_layer::events_panel_dock_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.20f, nullptr,
                                                                      &dock_main_id);
      
      ImGui::DockBuilderDockWindow("Viewport", dock_main_id);
      ImGui::DockBuilderDockWindow("Scene Editor", ImGui_layer::scene_panel_dock_id);
      ImGui::DockBuilderDockWindow("System Info", ImGui_layer::events_panel_dock_id);
      ImGui::DockBuilderDockWindow("Properties", ImGui_layer::info_panel_dock_id);
      ImGui::DockBuilderFinish(dock_main_id);
      ImGui_layer::viewport_panel_dock_id = dock_main_id;
    }
    
    dockspace_flags |= ImDrawListFlags_AntiAliasedLinesUseTex;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
      ImGui_layer::dockSpace_id = ImGui::GetID("Wave-Engine-DockSpace");
      ImGui::DockSpace(ImGui_layer::dockSpace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    
    if (ImGui::BeginMenuBar())
    {
      if (ImGui::BeginMenu("File"))
      {
        ImGui::MenuItem("Open Project...", "Ctrl+O");
        ImGui::MenuItem("New Scene", "Ctrl+N");
        ImGui::MenuItem("Save Scene", "Ctrl+S");
        ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S");
        
        if (ImGui::MenuItem("Exit", "Alt+F4"))
        {
          auto app = Engine::get_app();
          app->on_destroy();
          int exit_status = Engine::get_exit_status();
          exit(exit_status);
        }
        ImGui::EndMenu();
      }
    }
    
    if (ImGui::BeginMenu("Edit"))
    {
      ImGui::MenuItem("Add Component", "Ctrl+Alt+A");
      ImGui::MenuItem("Remove Component", "Ctrl+Alt+Del");
      ImGui::EndMenu();
    }
    
    if (ImGui::BeginMenu("View"))
    {
      if (ImGui::MenuItem("Toggle Scene Panel", "Ctrl+Alt+Del\t\t\t"))
        ImGui_layer::show_scene_panel = !ImGui_layer::show_scene_panel;
      
      ImGui::SameLine();
      ImGui::Text("%s", ImGui_layer::show_scene_panel ? "ON" : "OFF");
      
      if (ImGui::MenuItem("Toggle System Panel", "Ctrl+Alt+Del\t\t\t"))
        ImGui_layer::show_system_panel = !ImGui_layer::show_system_panel;
      
      ImGui::SameLine();
      ImGui::Text("%s", ImGui_layer::show_system_panel ? "ON" : "OFF");
      
      if (ImGui::MenuItem("Zoom", "Ctrl+Wheel-Up\t\t\t"))
      {
        ImGui_layer::imgui_data.font_scale += 0.10;
        io.FontGlobalScale = ImGui_layer::imgui_data.font_scale;
      }
      ImGui::SameLine();
      ImGui::Text("%d %%", (int) (io.FontGlobalScale * 100.0f));
      
      if (ImGui::MenuItem("Un-zoom", "Ctrl+Wheel-Down\t\t\t"))
      {
        ImGui_layer::imgui_data.font_scale -= 0.10;
        io.FontGlobalScale = ImGui_layer::imgui_data.font_scale;
      }
      ImGui::SameLine();
      ImGui::Text("%d %%", (int) (io.FontGlobalScale * 100.0f));
      ImGui::EndMenu();
    }
    
    if (ImGui::BeginMenu("About"))
    {
      ImGui::MenuItem("Check for Update");
      if (ImGui::MenuItem("About Wave Engine"))
      {
        if (ImGui::BeginPopup("About Wave Engine", ImGuiWindowFlags_ChildWindow))
        {
          ImGui::Text("\tVersion number : 1.10\n\tLast update : 2023-06-06");
          ImGui::EndPopup();
        }
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
    
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
  
  void ImGui_layer::draw_property(const std::string &label, Vector_2f &values, Vector_2f min_value, Vector_2f max_value,
                                  Vector_2f speed_value, Vector_2f reset_value, float column_width)
  {
    ImGuiIO &io = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[1];
    
    ImGui::PushID(label.c_str());
    
    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, column_width);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();
    
    ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});
    
    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = {lineHeight + 5.0f, lineHeight};
    
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    ImGui::PushFont(boldFont);
    
    if (ImGui::Button("X", buttonSize)) values.set_x(reset_value.get_x());
    
    ImGui::PopFont();
    ImGui::PopStyleColor(3);
    
    ImGui::SameLine();
    ImGui::DragFloat("##X", &values[0], speed_value.get_x(), min_value.get_x(), max_value.get_x(), "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();
    
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    ImGui::PushFont(boldFont);
    
    if (ImGui::Button("Y", buttonSize)) values.set_y(reset_value.get_y());
    
    ImGui::PopFont();
    ImGui::PopStyleColor(3);
    
    ImGui::SameLine();
    ImGui::DragFloat("##Y", &values[1], speed_value.get_y(), min_value.get_y(), max_value.get_y(), "%.2f");
    ImGui::PopItemWidth();
    
    ImGui::PopStyleVar();
    
    ImGui::Columns(1);
    
    ImGui::PopID();
  }
  
  void
  ImGui_layer::display_property(const std::string &label, Vector_3f &values, Vector_3f min_value, Vector_3f max_value,
                                Vector_3f speed_value, Vector_3f reset_value, float column_width)
  {
    ImGuiIO &io = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];
    
    ImGui::PushID(label.c_str());
    
    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, column_width);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();
    
    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});
    
    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};
    
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    ImGui::PushFont(boldFont);
    
    if (ImGui::Button("X", buttonSize)) values.set_x(reset_value.get_x());
    
    ImGui::PopFont();
    ImGui::PopStyleColor(3);
    
    ImGui::SameLine();
    
    ImGui::DragFloat("##X", &values[0], speed_value.get_x(), min_value.get_x(), max_value.get_x(), "%.2f");
    
    ImGui::PopItemWidth();
    ImGui::SameLine();
    
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    ImGui::PushFont(boldFont);
    
    if (ImGui::Button("Y", buttonSize)) values.set_y(reset_value.get_y());
    
    ImGui::PopFont();
    ImGui::PopStyleColor(3);
    
    ImGui::SameLine();
    
    ImGui::DragFloat("##Y", &values[1], speed_value.get_y(), min_value.get_y(), max_value.get_y(), "%.2f");
    
    ImGui::PopItemWidth();
    ImGui::SameLine();
    
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    ImGui::PushFont(boldFont);
    
    if (ImGui::Button("Z", buttonSize)) values.set_z(reset_value.get_z());
    
    ImGui::PopFont();
    ImGui::PopStyleColor(3);
    
    ImGui::SameLine();
    
    ImGui::DragFloat("##Z", &values[2], speed_value.get_z(), min_value.get_z(), max_value.get_z(), "%.2f");
    
    ImGui::PopItemWidth();
    
    ImGui::PopStyleVar();
    
    ImGui::Columns(1);
    
    ImGui::PopID();
  }
  
  void ImGui_layer::set_colors()
  {
    auto &colors = ImGui::GetStyle().Colors;
    
    auto dark_grey = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    auto light_grey = ImVec4(0.23f, 0.23f, 0.23f, 1.0f);
    auto extra_light_grey = ImVec4{0.3f, 0.3f, 0.3f, 1.0f};
    
    // Window Background.
    colors[ImGuiCol_WindowBg] = ImVec4{0.185f, 0.185f, 0.185f, 1.0f};
    
    // Title Background.
    colors[ImGuiCol_TitleBg] = light_grey;
    colors[ImGuiCol_TitleBgActive] = light_grey;
    colors[ImGuiCol_TitleBgCollapsed] = extra_light_grey;
    
    // Headers
    colors[ImGuiCol_Header] = light_grey;
    colors[ImGuiCol_HeaderHovered] = extra_light_grey;
    colors[ImGuiCol_HeaderActive] = light_grey;
    
    // Frame BG
    colors[ImGuiCol_FrameBg] = dark_grey;
    colors[ImGuiCol_FrameBgHovered] = light_grey;
    colors[ImGuiCol_FrameBgActive] = dark_grey;
    
    // Tabs
    colors[ImGuiCol_Tab] = light_grey;
    colors[ImGuiCol_TabHovered] = extra_light_grey;
    colors[ImGuiCol_TabActive] = light_grey;
    colors[ImGuiCol_TabUnfocused] = extra_light_grey;
    colors[ImGuiCol_TabUnfocusedActive] = light_grey;
    
    // Buttons
    colors[ImGuiCol_Button] = light_grey;
    colors[ImGuiCol_ButtonHovered] = extra_light_grey;
    colors[ImGuiCol_ButtonActive] = dark_grey;
  }
  
  [[maybe_unused]] uint32_t ImGui_layer::get_active_widget_id()
  {
    return GImGui->ActiveId;
  }
}