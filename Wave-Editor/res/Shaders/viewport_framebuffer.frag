#version 420 core

layout (location = 0) in vec2 vout_tex_coords;

layout (binding = 2) uniform sampler2DMS u_color_attachment_sampler;
uniform int u_viewport_width = 1920;
uniform int u_viewport_height = 1080;
uniform int u_max_samples = 1;

layout (location = 0) out vec4 fout_color;

void main() {
    //texelFetch requires a vec of ints for indexing (since we're indexing pixel locations)\n"
    //texture coords is range [0, 1], we need range [0, viewport_dim].\n"
    //texture coords are essentially a percentage, so we can multiply text coords by total size \n"
    ivec2 viewport_coords = ivec2(u_viewport_width, u_viewport_height);
    viewport_coords.x = int(vpCoords.x * vout_tex_coords.x);
    viewport_coords.y = int(vpCoords.y * vout_tex_coords.y);
    fout_color = texelFetch(u_color_attachment_sampler, viewport_coords, u_max_samples - 1);
}