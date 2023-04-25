#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 tex_coords;
out vec2 vout_tex_coords;

uniform mat4 u_transform;

void main()
{
  gl_Position = u_transform * vec4(position, 0.0, 1.0);
  vout_tex_coords = tex_coords;
}