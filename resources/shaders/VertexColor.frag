/**
 * @file VertexColor.frag
 * Fragment shader for basic Blinn-Phong lighting of a surface.
 * Calculates lit fragment color from view position and world-space vertex data.
 */

 // Used by a live compilation plugin I use in MSVS.
//! #include "Lights.frag"

/** Position of the camera in world space. */
uniform vec3 viewPosition;

/** Vertex position in world space. */
in vec3 vert_pos;
/** Vertex normal in world space. */
in vec3 vert_normal;

/** Final fragment color. */
out vec4 color;

/**
 * Outputs lit color based on material and light properties.
 */
void main()
{
    // Normalize the vertex normals just in case.
    vec3 normal = normalize(vert_normal);
    // Calculate the view direction from camera and vertex position.
    vec3 viewDir = normalize(vert_pos - viewPosition);

    // Calculate the light contribution and use it as the final color.
    color = vec4(calculateTotalLight(directionalLight, pointLights, normal, vert_pos, viewDir), 1.0);
}