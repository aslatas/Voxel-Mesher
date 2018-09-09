/**
 * @file VertexColor.vert
 * Trivial vertex shader uses projection matrices and vertex data to generate 
 * position and normal data for use in the fragment shader.
 */

#version 410

/** Local to World transform. */
uniform mat4 model;
/** World to View transform. */
uniform mat4 view;
/** View to Clip transform. */
uniform mat4 projection;

/** Vertex position in local space. */
in vec3 vertexPosition;
/** Vertex normal in local space. */
in vec3 vertexNormal;

/** Vertex position in world space. */
out vec3 vert_pos;
/** Vertex normal in world space. */
out vec3 vert_normal;

/**
 * Applies world transform in order to get world position and normal of vertex
 * data. Transforms position into clip space to set GL position.
 */
void main() 
{
    vert_pos = vec3(model * vec4(vertexPosition, 1.0));
    vert_normal = vec3(model * vec4(vertexNormal, 1.0));

    gl_Position = projection * view * vec4(vert_pos, 1.0);
}