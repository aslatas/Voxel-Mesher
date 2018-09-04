#version 410

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

in vec3 frag_color;
in vec3 frag_normal;
in vec3 frag_pos;

out vec4 color;

void main()
{
    float ambientStrength = 0.08;
    float specularStrength = 0.5;

    vec3 normal = normalize(frag_normal);
    vec3 lightDir = normalize(lightPos - frag_pos);
    vec3 viewDir = normalize(viewPos - frag_pos);
    vec3 halfDir = normalize(lightDir + viewDir);

    vec3 ambient = ambientStrength * lightColor;
    vec3 diffuse = max(dot(normal, lightDir), 0.0f) * lightColor;
    vec3 specular = specularStrength * lightColor * pow(max(dot(viewDir, halfDir), 0.0), 32);
    color = vec4((ambient + diffuse + specular) * frag_color, 1.0);

}