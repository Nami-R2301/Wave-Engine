#version 330 core

layout (location = 0) in int in_entity_id;
layout (location = 1) in vec2 in_position;
layout (location = 2) in vec4 in_color;
layout (location = 3) in vec2 in_tex_coords;

out vec2 vout_tex_coords;
out vec4 vout_color;
flat out int vout_entity_id;

uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * vec4(in_position, 0.0, 1.0);
    vout_tex_coords = in_tex_coords;
    vout_color = in_color;
    vout_entity_id = in_entity_id;
}