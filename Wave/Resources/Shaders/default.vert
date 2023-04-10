#version 330 core

// Vertex attributes.
layout (location = 0) in vec3 in_vertex_position;
layout (location = 1) in vec3 in_vertex_normal;
layout (location = 2) in vec4 in_color;
layout (location = 3) in vec2 in_texCoord;

// MVP matrix.
uniform mat4 u_transform;

// Input variables.
uniform vec3 u_mouse_pos;

// Outputs.
out vec2 vout_texCoord0;
out vec4 vout_frag_color;

void main()
{
  gl_Position = u_transform * vec4(in_vertex_position.x + (gl_InstanceID * 4), in_vertex_position.y, in_vertex_position.z, 1.0);
  vout_texCoord0 = in_texCoord;
  vout_frag_color = in_color;
}