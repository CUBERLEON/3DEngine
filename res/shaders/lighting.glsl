struct Attenuation
{
    float constant;
    float linear;
    float exponent;
};

struct DirectionalLight
{
    vec3 color;
    float intensity;
    vec3 direction;
};

struct PointLight
{
    vec3 color;
    float intensity;
    Attenuation attenuation;
    vec3 position;
    float range;
};

struct SpotLight
{
    PointLight pointLight;
    vec3 direction;
    float cutoff;
};

vec3 calcLight(vec3 color, float intensity, vec3 direction, vec3 normal, vec3 worldPosition)
{
    float diffuseFactor = dot(normal, -direction);

    vec3 diffuseColor = vec3(0, 0, 0);
    vec3 specularColor = vec3(0, 0, 0);

    if (diffuseFactor > 0 && intensity > 0)
    {
        diffuseColor = color * intensity * diffuseFactor;

        vec3 directionToEye = normalize(o_eyePosition - worldPosition);
        vec3 reflectedDirection = normalize(reflect(direction, normal));

        float cosAngleIncidence = dot(directionToEye, reflectedDirection);
        cosAngleIncidence = clamp(cosAngleIncidence, 0, 1);

/*      vec3 halfAngle = normalize(-direction + directionToEye);
        float blinnTerm = dot(normal, halfAngle);
        blinnTerm = clamp(blinnTerm, 0, 1);
        blinnTerm = cosAngleIncidence != 0.0 ? blinnTerm : 0.0;
        blinnTerm = pow(blinnTerm, m_specularPower);
        
        specularColor = vec4(color, 1) * m_specularIntensity * blinnTerm; //blinn-phong specular lighting
*/

        float specularFactor = pow(cosAngleIncidence, m_specularPower);
        specularColor = color * texture2D(m_specularMap, f_textureCoord).xyz * specularFactor; //phong specular lighting
        specularColor *= clamp(dot(reflectedDirection, normal), 0, 1);
    }

    return diffuseColor + specularColor;
}

vec3 calcDirectionalLight(DirectionalLight directionalLight, vec3 normal, vec3 worldPosition)
{
    return calcLight(directionalLight.color, directionalLight.intensity, directionalLight.direction, normal, worldPosition);
}

vec3 calcPointLight(PointLight pointLight, vec3 normal, vec3 worldPosition)
{
    vec3 lightDirection = worldPosition - pointLight.position;
    float distanceToPoint = length(lightDirection);

    if (distanceToPoint > pointLight.range)
        return vec3(0, 0, 0);

    lightDirection = normalize(lightDirection);

    vec3 color = calcLight(pointLight.color, pointLight.intensity, lightDirection, normal, worldPosition);

    float attenuation = max(0.00001, pointLight.attenuation.constant +
                                     pointLight.attenuation.linear * distanceToPoint +
                                     pointLight.attenuation.exponent * distanceToPoint * distanceToPoint);

    return color / attenuation;
}


vec3 calcSpotLight(SpotLight spotLight, vec3 normal, vec3 worldPosition)
{
    vec3 lightDirection = normalize(worldPosition - spotLight.pointLight.position);
    float spotFactor = dot(lightDirection, spotLight.direction);

    vec3 color = vec3(0, 0, 0);

    if (spotFactor > spotLight.cutoff && spotLight.cutoff != 1.0)
    {
        color = calcPointLight(spotLight.pointLight, normal, worldPosition) *
                (1.0 - (1.0 - spotFactor) / (1.0 - spotLight.cutoff));
    }

    return color;
}