//
// Created by nami on 9/5/22.
//

#pragma once

#ifdef GLEW_STATIC
#include <GL/glew.h>

#endif

#ifdef GLFW3_STATIC
#include <GLFW/glfw3.h>

#endif

#include <Core/layer.h>
#include <Renderer/gl_buffer.h>
#include <Renderer/gl_vertex_array_buffer.h>
#include <Objects/object.h>
#include <Objects/text.h>
#include <Events/renderer_event.h>
#include <Window/window.h>

namespace Wave
{
  
  class Renderer
  {
    static void init();
    static void shutdown();
    
    static void begin_scene(Camera &camera);
    static void end_scene();
    
    static void
    send_to_gpu(const std::shared_ptr<Gl_shader> &shader, const std::shared_ptr<Vertex_array_buffer> &vertexArray,
                const Matrix_4f &transform = Matrix_4f());
  };
  
  class Gl_renderer : public Layer // OpenGL renderer.
  {
  public:
    [[nodiscard]] static Renderer_state get_state();
    [[nodiscard]] static Renderer_api get_api();
    [[nodiscard]] static const std::function<void(Event &event)> &get_event_callback_function();
    
    static void set_state(Renderer_state new_state);
    static void set_event_callback_function(const std::function<void(Event &event)> &event_callback_function_);
    
    // Initial setup.
    static void init();
    static bool is_running();
    void on_event(Event &event) override;
    static void on_window_resize(Window *window, float width, float height);
    static const char *get_gl_version();
    static void show_renderer_info();
    static void enable_textures(bool enabled);
    
    // Frame changes.
    static void clear_bg();
    static void set_viewport(float width, float height);
    static void set_clear_color(const Color &color);
    
    // Loading objects.
    static void load_dynamic_data(const void *vertices, size_t size, uint64_t vbo_index = 0);
    static void load_object(const Object_3D *object);
    [[nodiscard]] static std::shared_ptr<Gl_vertex_array_buffer> load_text();
    
    // Rendering objects.
    static void draw_object(const Object_3D *object);
    static void draw_objects(const std::vector<Object_3D> *objects);
    static void draw_loaded_objects(uint32_t object_count);
    static void draw_text(const std::shared_ptr<Text> &text, const std::shared_ptr<Vertex_array_buffer> &vao);
    
    // Error handling.
    static bool renderer_error_callback([[maybe_unused]] On_renderer_error &renderer_error);
    static void gl_clear_errors();
    static void gl_error_callback(unsigned int error_code, const char *error_message,
                                  const char *function_name, const char *file_name, size_t line_number);
    
    // Cleanup.
    static void delete_gl_buffers();
    static void shutdown();
  private:
    static Renderer_state state;
    static bool running;
    static Renderer_api api;
    static std::shared_ptr<Gl_vertex_array_buffer> vertex_array_buffers;
    static std::vector<Texture> textures;
    static std::function<void(Event &event)> event_callback_function;
    
    static void draw_object();
  };
}
