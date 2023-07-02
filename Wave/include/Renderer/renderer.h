//
// Created by nami on 9/5/22.
//

#pragma once

#if defined (GLEW_STATIC)

#include <glew.h>

#endif

#include <Layer/layer.h>
#include <Objects/object.h>
#include <Text//text.h>

#include <Utilities/resource_loader.h>
#include <Events/renderer_event.h>
#include <Renderer/shader.h>
#include <Renderer/vertex_array_buffer.h>
#include <Window/window.h>
#include <Renderer/framebuffer.h>
#include <Renderer/uniform_buffer.h>
#include <Scene/camera.h>

namespace Wave
{
  class Renderer
  {
    public:
    typedef struct Renderer_stats_s
    {
      uint64_t shaders_count = 0;
      uint64_t vao_count = 0;
      uint64_t vertices_drawn_count = 0;
      uint64_t indices_drawn_count = 0;
      uint64_t textures_drawn_count = 0;
      uint64_t draw_call_count = 0;
      uint64_t object_count = 0;
      uint64_t text_glyph_count = 0;
    } Renderer_stats_s;
    
    typedef struct Entity_draw_command_s
    {
      uint64_t instance_count = 1;
      uint64_t vertex_count = 0, index_count = 0;
      uint64_t vertex_size = 0, index_size = 0;
      uint64_t ibo_offset = 0, base_vertex = 0;
      const void *vertex_data = nullptr, *index_data = nullptr;
    } Entity_draw_command_s;
    
    typedef struct Shader_draw_command_s
    {
      uint64_t batch_total_vbo_size = 0, batch_total_ibo_size = 0;
      std::unordered_map<uint64_t, Entity_draw_command_s> entity_commands;
      Shader *associated_shader = nullptr;
      std::shared_ptr<Vertex_array_buffer> vertex_array_buffer;
    } Shader_draw_command_s;
    
    public:
    static void create(Renderer_api api);
    static void init();
    static void toggle_wireframe(bool enable_wireframe);
    
    [[nodiscard]] static Renderer_stats_s get_stats();
    [[nodiscard]] static Renderer_api get_api();
    [[nodiscard]] static const char *get_api_version();
    [[nodiscard]] static const char *get_api_shader_version();
    [[nodiscard]] static bool is_running();
    static void set_event_callback_function(const std::function<void(Event_system::Event &)> &callback);
    
    // Error handling
    static void clear_errors();
    static Renderer_state get_state();
    static int32_t get_max_texture_units();
    
    // Framebuffer manipulations
    static void clear_bg();
    static void set_clear_color(const Color &bg_color);
    static void set_viewport(const Math::Vector_2f &viewport);
    
    // Events
    static void on_event(Event_system::Event &event);
    
    // Batch rendering.
    static void begin(std::shared_ptr<Camera> &camera);
    
    static void add_draw_command(uint64_t entity_id, Shader &shader, const void *vertices, uint64_t vertex_count,
                                 uint64_t vertex_size, const void *indices, uint64_t index_count,
                                 uint64_t index_size, Texture *texture_, bool flat_shaded);
    static void replace_draw_command(uint64_t entity_id, Shader &shader, const void *vertices, uint64_t vertex_count,
                                     uint64_t vertex_size, const void *indices, uint64_t index_count,
                                     uint64_t index_size, Texture *texture_);
    static void remove_draw_command(uint64_t shader_id, uint64_t entity_id);
    static void flush();
    
    static void end();
    
    // Shutdown
    static void shutdown();
    static bool has_crashed();
    private:
    static Renderer_api api_in_use;
  };
}
