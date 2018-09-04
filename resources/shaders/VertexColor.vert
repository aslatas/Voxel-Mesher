#version 410

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 pos;
in vec3 normal;
in vec3 color;

out vec3 frag_color;
out vec3 frag_normal;
out vec3 frag_pos;

void main() 
{
    frag_color = color;
    frag_pos = vec3(model * vec4(pos, 1.0));
    frag_normal = vec3(model * vec4(normal, 1.0));

    gl_Position = projection * view * model * vec4(pos, 1.0);
}