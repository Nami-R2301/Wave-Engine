#version 330 core

// Vertex attributes.
layout (location = 0) in vec3 in_vertex_position;
layout (location = 1) in vec3 in_vertex_normal;
layout (location = 2) in vec4 in_color;
layout (location = 3) in vec2 in_tex_coords;

// Camera matrix.
layout (std140) uniform u_camera
{
    mat4 u_view;
    mat4 u_projection;
};

uniform mat4 u_model;

// Input variables.
uniform vec3 u_mouse_pos;

// Outputs.
out vec2 vout_tex_coords;
out vec4 vout_frag_color;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(in_vertex_position.xyz, 1.0);
    vout_tex_coords = in_tex_coords;
  vout_frag_color = in_color;
}