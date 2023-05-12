//
// Created by nami on 9/5/22.
//

#pragma once

#if defined (GLEW_STATIC)

#include <glew.h>

#endif

#include <Core/layer.h>
#include <Objects/objects.h>
#include <Objects/text.h>

#include <Events/renderer_event.h>

#include <Renderer/shader.h>
#include <Renderer/vertex_array_buffer.h>
#include <Window/window.h>
#include <Renderer/framebuffer.h>
#include <Renderer/uniform_buffer.h>

#ifndef DEBUG

#define LOG_TASK(category, color, number, start_msg, event, end_msg)\
    event;

#define LOG_INSTRUCTION(category, color, start_msg, instruction)\
    instruction;

#else

// Macro to log engine tasks and events to monitor app in debug mode (logger.h).
#define LOG_TASK(category, color, number, start_msg, event, end_msg)\
      Wave::alert(WAVE_STATUS_IN_PROGRESS, "[%s] [%s#%d%s] --> %s", category, color, number, DEFAULT, start_msg); \
    event;\
    if (!Wave::Renderer::has_crashed())                                  \
    Wave::alert(WAVE_TASK_DONE, "[%s] [%s#%d%s] --> %s %sSuccessfully%s", category, color, number, DEFAULT, end_msg, GREEN, DEFAULT); \
    else Wave::alert(WAVE_TASK_FAIL, "[%s] [%s#%d%s] --> %s %sUnsuccessfully%s", category, color, number, DEFAULT, end_msg, RED, DEFAULT);\

#define LOG_INSTRUCTION(category, color, start_msg, instruction)\
    Wave::alert(WAVE_STATUS_IN_PROGRESS, "[%s%s%s] --> %s", color, category, DEFAULT, start_msg); \
    instruction; \
    Wave::alert(WAVE_INSTRUCTION_DONE);
#endif  // Debug

constexpr int max_draw_commands = 1000;

namespace Wave
{
  
  class Renderer
  {
    public:
    typedef struct Draw_commands_list
    {
      std::vector<std::string> names;
      std::vector<std::shared_ptr<Texture>> textures;
      std::vector<std::shared_ptr<Vertex_array_buffer>> vertex_array_buffers;
      std::vector<std::shared_ptr<Uniform_buffer>> uniform_buffers;
    } Draw_commands_list;
    
    public:
    static void create(Renderer_api api);
    static void init();
    static Renderer_api get_api();
    static bool is_running();
    static void set_event_callback_function(const std::function<void(Event &)> &callback);
    
    // Error handling
    static void clear_errors();
    static Renderer_state get_state();
    
    // Framebuffer manipulations
    static void clear_bg();
    static void set_clear_color(const Color &bg_color);
    static void set_viewport(const Vector_2f &viewport);
    
    // Events
    static void on_event(Event &event);
    
    // Scene setup
    static void begin_scene(Camera &camera);
    
    template<typename T>
    static void add(T objects)
    {
    }
    
    static void end_scene();
    
    // Load assets
    static std::shared_ptr<Vertex_array_buffer> load_text();
    [[maybe_unused]] [[maybe_unused]] static void
    load_dynamic_data(const void *vertices, size_t size, uint64_t vbo_index = 0);
    static void load_object(const Object_3D *object);
    
    // Static rendering.
    static void draw_object(const Object_3D *object);
    [[maybe_unused]] static void draw_objects(const std::vector<Object_3D> *objects);
    static void draw_loaded_objects(uint32_t object_count);
    static void draw_text(const std::shared_ptr<Text> &text, const std::shared_ptr<Vertex_array_buffer> &vao);
    
    // Batch rendering.
    static void flush();
    static void send(const std::shared_ptr<Shader> &shader,
                     const std::shared_ptr<Vertex_array_buffer> &vertexArray,
                     const Matrix_4f &transform = Matrix_4f());
    
    // Shutdown
    static void shutdown();
    static bool has_crashed();
    private:
    static Renderer_api api_in_use;
  };
}
