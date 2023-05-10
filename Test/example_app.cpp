//
// Created by nami on 8/26/22.
//

#include <example_app.h>
#include <example_layer.h>
// NEED TO BE INCLUDED IN MAIN APP ONLY
#include <entrypoint.h>

Example_app::Example_app() : Wave::Engine(Wave::Renderer_api::Opengl)
{
    // Add Cameras
    this->demo_perspective_camera = Wave::create_shared_pointer<Wave::Perspective_camera>(90.0f, 0.1f, 1000.0f);

    // Add shaders
    this->demo_shaders.emplace_back(Wave::Shader::create("Default",
        Wave::Res_loader_3D::load_shader_source(
            "default.vert").c_str(),
        Wave::Res_loader_3D::load_shader_source(
            "default.frag").c_str()));


    // Add objects
    this->demo_objects.emplace_back(Wave::create_shared_pointer<Wave::Object_3D>(Wave::Res_loader_3D("awp.obj").load_3D_mesh()));
    this->demo_objects.emplace_back(Wave::create_shared_pointer<Wave::Object_3D>(Wave::Res_loader_3D("cube.obj").load_3D_mesh()));

    push_layer(new Example_scene_3D(this->demo_perspective_camera,
        this->demo_shaders,
        this->demo_objects[0]));
}

Example_app::~Example_app() = default;

void Example_app::init()
{

}

void Example_app::on_update(float time_step)
{
    Engine::on_update(time_step);
}

void Example_app::on_event(Wave::Event& event)
{
    Engine::on_event(event);

    Wave::Event_dispatcher event_dispatcher(event);

    switch (event.get_event_type())
    {
    case Wave::Event_type::On_key_event:
    {
        event_dispatcher.dispatch<Wave::On_any_key_event>(BIND_EVENT_FUNCTION(any_key_callback));
        break;
    }
    case Wave::Event_type::On_mouse_movement:
    {
        event_dispatcher.dispatch<Wave::On_mouse_movement>(BIND_EVENT_FUNCTION(mouse_movement_callback));
        break;
    }
    case Wave::Event_type::On_mouse_button_press:
    {
        event_dispatcher.dispatch<Wave::On_mouse_button_press>(BIND_EVENT_FUNCTION(mouse_button_press_callback));
        break;
    }
    case Wave::Event_type::On_mouse_button_hold:
    {
        event_dispatcher.dispatch<Wave::On_mouse_button_hold>(BIND_EVENT_FUNCTION(mouse_button_hold_callback));
        break;
    }
    case Wave::Event_type::On_mouse_button_release:
    {
        event_dispatcher.dispatch<Wave::On_mouse_button_release>(BIND_EVENT_FUNCTION(mouse_button_release_callback));
        break;
    }
    case Wave::Event_type::On_mouse_wheel_scroll:
    {
        event_dispatcher.dispatch<Wave::On_mouse_wheel_scroll>(BIND_EVENT_FUNCTION(mouse_wheel_callback));
        break;
    }
    default:break;
    }
}

bool Example_app::window_closed_callback([[maybe_unused]] Wave::On_window_close& window_closed_event)
{
    window_closed_event.print(Wave::Print_type::Warn);
    Engine::main_window->close();
    return true;
}

bool Example_app::mouse_movement_callback(Wave::On_mouse_movement& mouse_cursor_event)
{
    float sensitivity = 1.0f;
    auto current_mouse_position = mouse_cursor_event.get_mouse_cursor_position();

    if (Wave::Input::is_mouse_button_held(WAVE_MOUSE_BUTTON_LEFT))
    {
        float center_x = this->demo_perspective_camera->get_center_position().get_x();
        float center_y = this->demo_perspective_camera->get_center_position().get_y();

        float delta_x = current_mouse_position.get_x() - center_x;
        float delta_y = current_mouse_position.get_y() - center_y;

        float rotation_x = delta_x / (float)this->demo_perspective_camera->get_width();
        float rotation_y = delta_y > center_y ? (-delta_y / (float)this->demo_perspective_camera->get_height()) / 2 :
            (delta_y / (float)this->demo_perspective_camera->get_height()) / 2;

        this->demo_objects[0]->rotate(rotation_x * sensitivity, rotation_y * sensitivity, 0);
    }
    return true;
}

bool Example_app::mouse_wheel_callback(Wave::On_mouse_wheel_scroll& mouse_wheel_input)
{
    float sensitivity = 1.0f;
    if (mouse_wheel_input.get_mouse_wheel_offset() != Wave::Vector_2f(0, 0))
    {
        this->demo_perspective_camera->move(this->demo_perspective_camera->get_forward(),
            mouse_wheel_input.get_mouse_wheel_offset().get_y() * sensitivity);
    }
    return true;
}

bool Example_app::window_resize_callback(Wave::On_window_resize& window_resized_event)
{
    Engine::window_resize_callback(window_resized_event);
    this->demo_perspective_camera->on_window_resize(window_resized_event);
    return true;
}

Wave::Engine* Wave::create_app()
{
    return new Example_app();
}
