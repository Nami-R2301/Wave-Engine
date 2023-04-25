#version 330 core
in vec2 vout_tex_coords;
out vec4 fout_color;

uniform sampler2D u_text;
uniform vec4 u_text_color;

void main()
{
  vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_text, vout_tex_coords).r);
  fout_color = u_text_color * sampled;
}