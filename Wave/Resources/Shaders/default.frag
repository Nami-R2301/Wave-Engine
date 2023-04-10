#version 330 core

in vec4 vout_frag_color;// Imports the color from the Vertex Shader
in vec2 vout_texCoord0;// Texture coordinates.

uniform sampler2D u_sampler;
uniform bool u_has_texture = false;

void main()
{
  if (!u_has_texture) gl_FragColor = vout_frag_color;
  else
  {
    vec4 materialDiffuseColor = texture(u_sampler, vout_texCoord0) * vout_frag_color;
    gl_FragColor = materialDiffuseColor;
  }
}