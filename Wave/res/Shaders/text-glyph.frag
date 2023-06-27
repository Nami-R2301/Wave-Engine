#version 420 core

layout (binding = 11) uniform sampler2D u_sampler;

struct Vertex_data_s
{
    vec2 vout_tex_coords;
    vec4 vout_color;
};

layout (location = 0) in Vertex_data_s vout_vertex_data;
layout (location = 2) flat in int vout_entity_id;

layout (location = 0) out vec4 fout_color;
layout (location = 1) out int fout_entity_id;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_sampler, vout_vertex_data.vout_tex_coords).r);
    fout_color = vout_vertex_data.vout_color * sampled;
    fout_entity_id = vout_entity_id;
}