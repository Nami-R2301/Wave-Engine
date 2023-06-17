#version 330 core

in vec2 vout_tex_coords;
in vec4 vout_color;
flat in int vout_entity_id;

out vec4 fout_color;
out int fout_entity_id;

uniform sampler2D u_sampler;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_sampler, vout_tex_coords).r);
    fout_color = vout_color * sampled;
    fout_entity_id = vout_entity_id;
}