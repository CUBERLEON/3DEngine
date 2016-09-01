#version 330

out vec4 OutColor;

in vec2 f_textureCoord;
in vec3 f_normal;
in vec3 f_worldPosition;
in mat3 f_TBN;
in vec4 f_lightSpacePosition;

//additional variables
uniform vec3 o_eyePosition;
uniform sampler2D o_shadowMap;
//uniform sampler2DShadow o_shadowMap;

//material
uniform sampler2D m_diffuseMap;
uniform sampler2D m_normalMap;
uniform sampler2D m_specularMap;
uniform sampler2D m_alphaMap;
uniform vec3 m_specularColor;
uniform float m_specularPower;

include "lighting.glsl"

//lights
uniform DirectionalLight l_directional;
uniform vec3 l_ambientColor;

float texture2DCompare(sampler2D depths, vec2 uv, float compare)
{
    float depth = texture2D(depths, uv).r;
    //return step(depth, compare);
	return (depth < compare) ? 1. : 0.;
}
        
float texture2DShadowLerp(sampler2D depths, vec2 size, vec2 uv, float compare)
{
    vec2 texelSize = vec2(1.0) / size;
    vec2 f = fract(uv * size + 0.5);
    vec2 centroidUV = floor(uv * size + 0.5) / size;

    float lb = texture2DCompare(depths, centroidUV + texelSize*vec2(0.0, 0.0), compare);
    float lt = texture2DCompare(depths, centroidUV + texelSize*vec2(0.0, 1.0), compare);
    float rb = texture2DCompare(depths, centroidUV + texelSize*vec2(1.0, 0.0), compare);
    float rt = texture2DCompare(depths, centroidUV + texelSize*vec2(1.0, 1.0), compare);
    float a = mix(lb, lt, f.y);
    float b = mix(rb, rt, f.y);
    float c = mix(a, b, f.x);
    return c;
}

float PCF(sampler2D depths, vec2 size, vec2 uv, float compare)
{
    float result = 0.0;
	int k = 1;
	vec2 texelSize = vec2(1.0) / size;
    for (int x = -k; x <= k; ++x)
	{
        for (int y = -k; y <= k; ++y)
		{
            result += texture2DShadowLerp(depths, size, uv + texelSize * vec2(x, y), compare);
        }
    }
    return result / 9.0;
}

float linstep(float low, float high, float v)
{
	return clamp((v-low)/(high-low), 0.0, 1.0);
}

float VSM(sampler2D depths, vec2 uv, float compare)
{
	vec2 moments = texture2D(depths, uv).xy;
	float p = smoothstep(compare-0.02, compare, moments.x);
	float variance = max(moments.y - moments.x*moments.x, -0.001);
	float d = compare - moments.x;
	float p_max = linstep(0.2, 1.0, variance / (variance + d*d));
	return clamp(max(p, p_max), 0.0, 1.0);
}

void main()
{
	float alpha = texture2D(m_alphaMap, f_textureCoord).x;
	if (alpha < 0.2)
		discard;
		
	vec3 normal = texture2D(m_normalMap, f_textureCoord).xyz;
	normal = (2.0 * normal - 1.0);
	normal = normalize(f_TBN * normal);
	
	vec3 lightProjCoords = f_lightSpacePosition.xyz / f_lightSpacePosition.w;
	lightProjCoords = lightProjCoords * 0.5 + 0.5; 
	float curDepth = lightProjCoords.z;

	float shadow = 0;
	if (curDepth < 1 && lightProjCoords.x > 0 && lightProjCoords.y > 0 && lightProjCoords.x < 1 && lightProjCoords.y < 1)
	{
		float bias = 2 * max(0.005 * clamp(1.0 - dot(f_normal, -l_directional.direction), 0, 1), 0.0005);
		float maxDepth = texture2D(o_shadowMap, lightProjCoords.xy).x;

		//shadow = (curDepth - bias > maxDepth) ? 1 : 0;
		//shadow = PCF(o_shadowMap, textureSize(o_shadowMap, 0), lightProjCoords.xy, curDepth - bias);
		//shadow = 1-VSM(o_shadowMap, lightProjCoords.xy, curDepth - bias);
		//shadow = 1. - texture(o_shadowMap, vec3(lightProjCoords.xy, curDepth - bias), 0); //7.34ms sampler2DShadow
		shadow = texture2DShadowLerp(o_shadowMap, textureSize(o_shadowMap, 0), lightProjCoords.xy, curDepth - bias); //7.7ms
	}

	vec3 light = l_ambientColor;
	if (shadow < 1)
		light = l_ambientColor + (1. - shadow) * calcDirectionalLight(l_directional, normal, f_worldPosition);

	OutColor = texture2D(m_diffuseMap, f_textureCoord) * vec4(light, 1);
	//OutColor.a *= alpha;

	//OutColor = texture2D(m_diffuseMap, f_textureCoord);
	//OutColor = texture2D(m_specularMap, f_textureCoord);
	//OutColor = vec4(f_normal, 1);
}
