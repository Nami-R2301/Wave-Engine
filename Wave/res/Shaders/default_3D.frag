#version 330 core

in vec4 vout_frag_color;// Imports the color from the Vertex Shader
in vec2 vout_tex_coords;// Texture coordinates.

out vec4 fout_frag_color;

uniform sampler2D u_sampler;
uniform bool u_has_texture = false;

void main()
{
    if (!u_has_texture) fout_frag_color = vout_frag_color;
    else
    {
        vec4 materialDiffuseColor = texture(u_sampler, vout_tex_coords) * vout_frag_color;
        fout_frag_color = materialDiffuseColor;
    }
}