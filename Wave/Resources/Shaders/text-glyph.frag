#version 330 core
in vec2 vout_tex_coords;
in vec4 vout_color;

uniform sampler2D u_text;

void main()
{
  vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_text, vout_tex_coords).r);
  gl_FragColor = vout_color * sampled;
}