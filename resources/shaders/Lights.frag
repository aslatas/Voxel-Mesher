/**
 * @file Lights.frag
 * Fragment shader preamble which defines structs, uniforms, and functions for
 * energy normalized Blinn-Phong lighting.
 * @TODO Maybe pull out defines into a utility preamble for math constants and 
 * conditional functions.
 */

#version 410

#define MAX_POINT_LIGHTS 4
#define M_PI 3.1415926535897932384626433832795

/** Number of point lights in the scene. Must be <= the max. */
uniform int numPointLights;

/** Material struct stores basic light interaction propertes. */
struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

/** Point Light struct stores light position, properties, and falloff. */
struct PointLight
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

/** Directional Light struct stores light direction and properties. */
struct DirectionalLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

/** Defines how the surface reacts to light. */
uniform Material material;
/** Directional light source. Only one allowed per scene. */
uniform DirectionalLight directionalLight;
/** Point light sources. Maximum count is pre-defined. */
uniform PointLight pointLights[MAX_POINT_LIGHTS];

/**
 * Calculates the contribution of a directional light on the surface.
 * @param light Directional light to examine.
 * @param fNorm Normal vector of this fragment, must be pre-normalized.
 * @param vDir View direction, must be pre-normalized.
 * @return Color contribution of the directional light on the surface.
 */
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 fNorm, vec3 vDir)
{
    // Energy normalization constant for the specular component of Blinn-Phong.
    float energyConstant = (8.0 + material.shininess) / (8.0 * M_PI);
    // Normalize and reverse the light direction.
    vec3 lightDir = normalize(-light.direction);
    // Calculate the half angle for use in specular calculation.
    vec3 halfDir = normalize(lightDir - vDir);

    // Calculate the ambient contribution.
    vec3 ambient = light.ambient * material.ambient;

    // Calculate diffuse contribution.
    vec3 diffuse = max(dot(fNorm, lightDir), 0.0) * material.diffuse * 
                   light.diffuse;

    // Calculate the specular contribution.
    vec3 specular = pow(max(dot(fNorm, halfDir), 0.0), material.shininess) * 
                    material.specular * light.specular * energyConstant;

    // Output the sum of ambient, diffuse, and specular contribution.
    return (ambient + diffuse + specular);
}

/**
 * Calculates the contribution of a point light on the surface.
 * @param light Point light to examine.
 * @param fNorm Normal vector of this fragment, must be pre-normalized.
 * @param vDir View direction, must be pre-normalized.
 * @return Color contribution of the point light on the surface.
 */
vec3 CalculatePointLight(PointLight light, vec3 fNorm, 
                                           vec3 fPos, 
                                           vec3 vDir)
{
    // Energy normalization constant for the specular component of Blinn-Phong.
    float energyConstant = (8.0 + material.shininess) / (8.0 * M_PI);
    // Calculate the light direction.
    vec3 lightDir = normalize(light.position - fPos);
    // Calculate the half angle for use in specular calculation.
    vec3 halfDir = normalize(lightDir - vDir);

    float dist = length(light.position - fPos);

    float attenuation = 1.0 / (light.constant + light.linear * dist + 
                        light.quadratic * (dist * dist));

    // Calculate the ambient contribution.
    vec3 ambient = light.ambient * material.ambient * attenuation;

    // Calculate diffuse contribution.
    vec3 diffuse = max(dot(fNorm, lightDir), 0.0) * material.ambient *
                   attenuation * light.diffuse;

    // Calculate the specular contribution.
    vec3 specular = pow(max(dot(fNorm, halfDir), 0.0), material.shininess) * 
                    material.specular * attenuation * light.specular * 
                    energyConstant;

    // Output the sum of ambient, diffuse, and specular contribution.
    return (ambient + diffuse + specular);
}

/**
 * Calculates the fragment color after lighting.
 * @param dLight The directional light in the scene.
 * @param pLights Array of point lights in the scene.
 * @param fNorm Normal vector of this fragment, must be pre-normalized.
 * @param fPos Position of this fragment in world space.
 * @param vDir View direction, must be pre-normalized.
 * @return Color contribution of all lights on the surface.
 */
vec3 calculateTotalLight(DirectionalLight dLight, 
                         PointLight pLights[MAX_POINT_LIGHTS], 
                         vec3 fNorm, vec3 fPos, vec3 vDir)
{
    // Calculate the directional light contribution.
    vec3 result = CalculateDirectionalLight(dLight, fNorm, vDir);

    // Add in the point light contributions.
    for (int i = 0; i < numPointLights; i++) {
        result += CalculatePointLight(pLights[i], fNorm, fPos, vDir);
    }

    // Output the sum.
    return result;
}