//
// Created by nami on 10/05/23.
//

#pragma once

#if defined (GLEW_STATIC)

#ifdef OPENGL_VERSION_4_3_PLUS  // OpenGL 4.3 and above required for async debug polling.

#define GL_ASYNC_ERROR_CALLBACK \
  static void gl_asynchronous_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, \
            const void *userParam);
            
#define CHECK_GL_CALL(x) x
#else

#if defined(DEBUG)

#define GL_ASYNC_ERROR_CALLBACK

// Otherwise use traditional synchronous debug polling

// OpenGL
#define CHECK_GL_CALL(x) if (Wave::Renderer::is_running()) Wave::Renderer::clear_errors(); x; \
  if (Wave::Renderer::is_running()) Gl_renderer::gl_synchronous_error_callback(glGetError(), "Unknown", #x, __FILE__, __LINE__)

#else

#define CHECK_GL_CALL(x) x
#define GL_ASYNC_ERROR_CALLBACK

#endif  // DEBUG

#endif  // OPENGL_VERSION_4_3_PLUS

#include <GL/glew.h>

#include <Renderer/renderer.h>
#include <Renderer/gl_buffer.h>
#include <Renderer/gl_framebuffer.h>
#include <Renderer/gl_vertex_array_buffer.h>
#include <Renderer/gl_shader.h>
#include <Renderer/gl_texture.h>

namespace Wave
{
  
  class Gl_renderer
  {
    public:
    Gl_renderer() = default;
    ~Gl_renderer() = default;
    
    [[nodiscard]] static bool is_running();
    [[nodiscard]] static bool has_crashed();
    [[nodiscard]] static int32_t get_max_texture_units();
    [[nodiscard]] static const Renderer_state &get_state();
    [[nodiscard]] static const Renderer_api &get_api();
    [[nodiscard]] static const char *get_api_version();
    [[nodiscard]] static const char *get_api_shader_version();
    [[nodiscard]] static const std::function<void(Event &event)> &get_event_callback_function();
    
    // Renderer state and API info.
    static void show_renderer_info();
    static void set_state(Renderer_state new_state);
    
    // Initial setup.
    static void init();
    
    // Events.
    static void on_event(Event &event);
    static void set_event_callback_function(const std::function<void(Event &event)> &event_callback_function_);
    
    // Frame changes.
    static void clear_bg();
    static void set_viewport(float width, float height);
    static void set_clear_color(const Color &color);
    
    // Rendering objects.
    static void begin(std::shared_ptr<Camera> &camera);
    static void send_object(const Object &object, Shader &linked_shader);
    static void send_text(const Text &text, Shader &linked_shader);
    static void flush();
    static void end();
    
    // Error handling.
    static bool renderer_error_callback([[maybe_unused]] On_renderer_error &renderer_error);
    static void gl_clear_errors();
    // Own implementation of an OpenGL debug output callback.
    static void gl_synchronous_error_callback(GLenum error_code, const char *error_message, const char *function_name,
                                              const char *file_name, size_t line_number);
    GL_ASYNC_ERROR_CALLBACK
    
    // Cleanup.
    static void shutdown();
    private:
    static Renderer_state state;
    static Renderer_api api;
    static Camera *scene_camera;
    static std::vector<Texture *> textures;
    // Map the draw commands to enable querying and overwriting with the name identifier.
    static std::vector<Renderer::Draw_command *> draw_commands;
    static std::vector<std::shared_ptr<Uniform_buffer>> uniform_buffers;
    static std::function<void(Event &event)> event_callback_function;
    private:
    // Loading assets and buffers.
    static void load_dynamic_vbo_data(const void *vertices, uint64_t count, uint64_t size, uint64_t command_index,
                                      uint64_t vbo_index = 0);
    static void load_dynamic_ibo_data(const void *faces, uint64_t count, uint64_t size, uint64_t command_index);
    static void init_object_buffers();
    static void init_text_buffers();
  };
}

#else

#error "Cannot use the OpenGL renderer API since Glew is not included! Did you forget to pass in GLEW_STATIC in your compile definitions?"

#endif
