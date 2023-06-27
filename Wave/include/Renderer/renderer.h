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
  
  constexpr int64_t c_max_vbo_buffer_size = 3'000'000;
  constexpr int64_t c_max_ibo_buffer_size = 5'000'000;
  
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
    
    typedef struct Offset_s
    {
      uint64_t instance_count = 1;
      uint64_t vertex_count = 0, index_count = 0;
      uint64_t vertex_size = 0, index_size = 0;
      uint64_t ibo_offset = 0, base_vertex = 0;
      const void *vertex_data = nullptr, *index_data = nullptr;
    } Offset_s;
    
    typedef struct Draw_command
    {
      uint64_t batch_total_vbo_size = 0, batch_total_ibo_size = 0;
      std::unordered_map<uint64_t, Offset_s> batch_offset;
      Shader *associated_shader = nullptr;
      std::shared_ptr<Vertex_array_buffer> vertex_array_buffer;
    } Draw_command;
    
    public:
    static void create(Renderer_api api);
    static void init();
    
    [[nodiscard]] static Renderer_stats_s get_stats();
    [[nodiscard]] static Renderer_api get_api();
    [[nodiscard]] static const char *get_api_version();
    [[nodiscard]] static const char *get_api_shader_version();
    [[nodiscard]] static bool is_running();
    static void set_event_callback_function(const std::function<void(Event &)> &callback);
    
    // Error handling
    static void clear_errors();
    static Renderer_state get_state();
    static int32_t get_max_texture_units();
    
    // Framebuffer manipulations
    static void clear_bg();
    static void set_clear_color(const Color &bg_color);
    static void set_viewport(const Math::Vector_2f &viewport);
    
    // Events
    static void on_event(Event &event);
    
    // Batch rendering.
    static void begin(std::shared_ptr<Camera> &camera);
    
    // For 3D objects.
    static void send_entity(uint64_t entity_id, Shader &shader, const std::vector<Vertex_3D> &vertices,
                            const std::vector<uint32_t> &indices,
                            std::vector<std::shared_ptr<Texture>> &textures, bool flat_shaded);
    // For 2D objects.
    static void send_entity(uint64_t entity_id, Shader &shader, const std::vector<Vertex_2D> &vertices,
                            const std::vector<uint32_t> &indices,
                            std::vector<std::shared_ptr<Texture>> &textures, bool flat_shaded);
    // For text.
    static void send_entity(uint64_t entity_id, Shader &shader, const std::vector<Glyph_quad_s> &vertices,
                            const std::vector<uint32_t> &indices, Texture &texture_atlas);
    
    static void replace_entity(uint64_t entity_id, Shader &shader, const std::vector<Vertex_2D> &vertices,
                               const std::vector<uint32_t> &indices,
                               std::vector<std::shared_ptr<Texture>> &textures);
    static void replace_entity(uint64_t entity_id, Shader &shader, const std::vector<Vertex_3D> &vertices,
                               const std::vector<uint32_t> &indices,
                               std::vector<std::shared_ptr<Texture>> &textures);
    static void replace_entity(uint64_t entity_id, Shader &shader, const std::vector<Glyph_quad_s> &vertices,
                               const std::vector<uint32_t> &indices, Texture &texture_atlas);
    
    static void free_entity(uint64_t shader_id, uint64_t entity_id);
    
    static void batch_data();
    static void flush();
    
    static void end();
    
    // Shutdown
    static void shutdown();
    static bool has_crashed();
    private:
    static Renderer_api api_in_use;
  };
}
