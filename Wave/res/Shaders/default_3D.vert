#version 330 core

// Vertex attributes.
layout (location = 0) in vec3 in_vertex_position;
layout (location = 1) in vec3 in_vertex_normal;
layout (location = 2) in vec4 in_color;
layout (location = 3) in vec2 in_tex_coords;
layout (location = 4) in mat4 in_model_matrix;

// Camera matrix.
layout (std140) uniform u_camera
{
    mat4 u_view;
    mat4 u_projection;
};

// Input variables.
uniform vec3 u_mouse_pos;

// Outputs.
out vec2 vout_tex_coords;
out vec4 vout_frag_color;
out vec3 vout_normal;
out vec3 vout_frag_position;
out vec4 vout_directional_light_position;


void main()
{
    gl_Position = u_projection * u_view * (in_model_matrix * vec4(in_vertex_position.xyz + (gl_InstanceID * 5), 1.0));
    vout_tex_coords = in_tex_coords;
    vout_frag_color = in_color;
    mat3 normal_matrix = mat3(transpose(inverse(u_view * in_model_matrix)));
    vout_normal = normalize(vec3(vec4(normal_matrix * in_vertex_normal, 0.0)));
    vout_frag_position = (in_model_matrix * vec4(in_vertex_position, 1.0)).xyz;
}