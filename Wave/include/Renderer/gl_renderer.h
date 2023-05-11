//
// Created by nami on 10/05/23.
//

#pragma once

#if defined (GLEW_STATIC)

#ifdef OPENGL_VERSION_4_3_PLUS  // Opengl 4.3 and above required for async debug polling.

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

namespace Wave
{
  
  class Gl_renderer
  {
    public:
    Gl_renderer() = default;
    ~Gl_renderer() = default;
    
    [[nodiscard]] static const Renderer_state &get_state();
    [[maybe_unused]] [[nodiscard]] static const Renderer_api &get_api();
    [[maybe_unused]] [[nodiscard]] static const std::function<void(Event &event)> &get_event_callback_function();
    
    static void set_state(Renderer_state new_state);
    static void set_event_callback_function(const std::function<void(Event &event)> &event_callback_function_);
    
    // Initial setup.
    static void init();
    [[nodiscard]] static bool is_running();
    static void begin_scene(Camera &camera);
    static void end_scene();
    static void flush();
    static void send(const Gl_shader &shader,
                     const Gl_vertex_array_buffer &vertexArray,
                     const Matrix_4f &transform = Matrix_4f());
    static void on_event(Event &event);
    [[maybe_unused]] static const char *get_gl_version();
    static void show_renderer_info();
    
    // Frame changes.
    static void clear_bg();
    static void set_viewport(float width, float height);
    static void set_clear_color(const Color &color);
    
    // Loading objects.
    [[maybe_unused]] static void load_dynamic_data(const void *vertices, size_t size, uint64_t vbo_index = 0);
    static void load_object(const Object_3D *object);
    [[nodiscard]] static std::shared_ptr<Vertex_array_buffer> load_text();
    
    // Rendering objects.
    static void draw_object(const Object_3D *object);
    [[maybe_unused]] static void draw_objects(const std::vector<Object_3D> *objects);
    static void draw_loaded_objects(uint32_t object_count);
    static void draw_text(const std::shared_ptr<Text> &text, const std::shared_ptr<Vertex_array_buffer> &vao);
    
    // Error handling.
    static bool renderer_error_callback([[maybe_unused]] On_renderer_error &renderer_error);
    static void gl_clear_errors();
    // Own implementation of an OpenGL debug output callback.
    static void gl_synchronous_error_callback(GLenum error_code, const char *error_message, const char *function_name,
                                              const char *file_name, size_t line_number);
    GL_ASYNC_ERROR_CALLBACK
    
    // Cleanup.
    static void delete_gl_buffers();
    static void shutdown();
    [[nodiscard]] static bool has_crashed();
    private:
    static Renderer_state state;
    static Renderer_api api;
    static std::shared_ptr<Vertex_array_buffer> vertex_array_buffers;
    static std::vector<Texture> textures;
    static std::function<void(Event &event)> event_callback_function;
    
    static void draw_object();
  };
}

#else

#error "Cannot use the OpenGL renderer API since Glew is not included! Did you forget to pass in GLEW_STATIC in your compile definitions?"

#endif
