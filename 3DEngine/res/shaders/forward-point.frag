#version 330

out vec4 OutColor;

in vec2 f_textureCoord;
in vec3 f_normal;
in vec3 f_worldPosition;
in mat3 f_TBN;

//additional variables
uniform vec3 o_eyePosition;

//material
uniform sampler2D m_diffuseMap;
uniform sampler2D m_normalMap;
uniform sampler2D m_specularMap;
uniform float m_specularIntensity;
uniform float m_specularPower;

include "lighting.glsl"

//lights
uniform PointLight l_point;

void main() {
    vec3 normal = texture2D(m_normalMap, f_textureCoord).xyz;
    normal = (2.0 * normal - 1.0);
    normal = normalize(f_TBN * normal);

    //OutColor = vec4(normal, 1);

    OutColor = texture2D(m_diffuseMap, f_textureCoord) * vec4(calcPointLight(l_point, normal, f_worldPosition), 1);
}
