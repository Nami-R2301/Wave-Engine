//
// Created by nami on 9/5/22.
//

#pragma once

#if defined (GLEW_STATIC)

#include <glew.h>

#endif

#include <Core/layer.h>
#include <Objects/object.h>
#include <Objects/text.h>

#include <Utilities/resource_loader.h>
#include <Events/renderer_event.h>
#include <Renderer/shader.h>
#include <Renderer/vertex_array_buffer.h>
#include <Window/window.h>
#include <Renderer/framebuffer.h>
#include <Renderer/uniform_buffer.h>
#include <Scene/camera.h>

constexpr int64_t max_draw_commands = 1000;
constexpr int64_t max_vertices = 2'000'000;
constexpr int64_t max_indices = 2'000'000;
constexpr int32_t max_textures = 32;

namespace Wave
{
  
  class Renderer
  {
    public:
    typedef struct Draw_command
    {
      uint64_t vbo_offset = 0, ibo_offset = 0;
      Shader *associated_shader = nullptr;
      std::shared_ptr<Vertex_array_buffer> vertex_array_buffer;
    } Draw_command;
    
    public:
    static void create(Renderer_api api);
    static void init();
    static Renderer_api get_api();
    static const char *get_api_version();
    static const char *get_api_shader_version();
    static bool is_running();
    static void set_event_callback_function(const std::function<void(Event &)> &callback);
    
    // Error handling
    static void clear_errors();
    static Renderer_state get_state();
    static int32_t get_max_texture_units();
    
    // Framebuffer manipulations
    static void clear_bg();
    static void set_clear_color(const Color &bg_color);
    static void set_viewport(const Vector_2f &viewport);
    
    // Events
    static void on_event(Event &event);
    
    // Load assets
    static void init_text_buffers();
    static void init_object_buffers();
    static void add_uniform_buffer(const char *uniform_block_name, uint64_t buffer_size);
    static void set_uniform_buffer_data();
    
    // Batch rendering.
    static void begin(std::shared_ptr<Camera> &camera);
    static void load_dynamic_data(const void *vertices, size_t size, uint64_t command_index, uint64_t vbo_index = 0);
    static void send_object(Object &object, Shader &linked_shader);
    static void send_text(Text &text, Shader &linked_shader);
    static void flush();
    
    static void end();
    
    // Shutdown
    static void shutdown();
    static bool has_crashed();
    private:
    static Renderer_api api_in_use;
  };
}
