#version 410 core

in vec3 vert_color;
in vec3 vert_normal;

uniform float specular_coef;

layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_normal;

void main()
{
    out_color.rgb = vert_color;
    out_color.a = specular_coef;
    out_normal = vec4(vert_normal, 1.f);
}