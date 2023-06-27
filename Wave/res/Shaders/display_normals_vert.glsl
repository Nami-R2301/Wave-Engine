#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in vec3 vout_normal[];

const float MAGNITUDE = 0.5f;

// Camera matrix.
layout (std140) uniform u_camera
{
    mat4 u_view;
    mat4 u_projection;
};

void generate_line(int index)
{
    gl_Position = u_projection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = u_projection * (gl_in[index].gl_Position + vec4(vout_normal[index], 0.0f) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    generate_line(0);// first vertex normal
    generate_line(1);// second vertex normal
    generate_line(2);// third vertex normal
}