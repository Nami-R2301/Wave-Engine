#version 330 core

layout (location = 0) in vec2 in_rectangle;
layout (location = 1) in vec2 in_tex_coords;

out vec2 vout_tex_coords;

uniform mat4 u_projection;

void main() {
    gl_Position = u_projection * vec4(in_rectangle.xy, 0.0f, 1.0f);
    vout_tex_coords = in_tex_coords;
}
