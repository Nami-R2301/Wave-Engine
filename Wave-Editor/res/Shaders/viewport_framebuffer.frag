#version 330 core

in vec2 vout_tex_coords;
out vec4 fout_color;

uniform sampler2DMS u_color_attachment_sampler;
uniform int u_viewport_width = 1920;
uniform int u_viewport_height = 1080;
uniform int u_max_samples = 1;

void main() {
    //texelFetch requires a vec of ints for indexing (since we're indexing pixel locations)\n"
    //texture coords is range [0, 1], we need range [0, viewport_dim].\n"
    //texture coords are essentially a percentage, so we can multiply text coords by total size \n"
    ivec2 vpCoords = ivec2(u_viewport_width, u_viewport_height);
    vpCoords.x = int(vpCoords.x * vout_tex_coords.x);
    vpCoords.y = int(vpCoords.y * vout_tex_coords.y);

    vec4 samples;
    for (int i = 0; i < u_max_samples; ++i)samples += texelFetch(u_color_attachment_sampler, vpCoords, i);
    vec4 average = samples / u_max_samples;
    fout_color = average;
}