#version 330

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;
in vec4 DirectionalLightPos;

out vec4 color;

struct Quadratic
{
    float constant;
    float linear;
    float quadratic;
};

struct Light
{
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight
{
    Light base;
    vec3 direction;
};

struct PointLight
{
    Light base;
    vec3 position;
    Quadratic attenuation;
    float farPlane;
};

struct SpotLight
{
    PointLight base;
    vec3 direction;
    float edge;
};

struct Material
{
    float specularIntensity;
    float shininess;
};

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

uniform sampler2D tex;
uniform sampler2D directionalShadowMap;

uniform DirectionalLight directionalLight;
uniform Material material;
uniform vec3 cameraPos;

uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform int pointLightCount;
uniform int spotLightCount;

uniform samplerCube omniDirectionalShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

vec3 sampleOffsetDirections[20] = vec3[]
(
    vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),
    vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
    vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
    vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
    vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

float Evaluate(Quadratic quadratic, float x)
{
    return quadratic.quadratic * x * x +
        quadratic.linear * x +
        quadratic.constant;
}

float CalcOmniDirectionalShadowFactor(PointLight light, int shadowIndex)
{
    vec3 fragToLight = FragPos - light.position;
    float currentDepth = length(fragToLight);

    float bias = 0.05;
    float shadow = 0;
    int samples = 20;

    float viewDistance = length(cameraPos - FragPos);
    float diskRadius = (1.0f + (viewDistance / light.farPlane)) / 25.0f;

    for (int i = 0; i < samples; i++)
    {
        float closestDepth = texture(omniDirectionalShadowMaps[shadowIndex], fragToLight + sampleOffsetDirections[i] * diskRadius).r * light.farPlane;
    
        if (currentDepth - bias > closestDepth)
        {
            shadow += 1.0f;
        }
    }

    return shadow / float(samples);
}

float CalcDirectionalShadowFactor(DirectionalLight light)
{
    vec3 projected = (DirectionalLightPos.xyz / DirectionalLightPos.w) * 0.5f + 0.5f;

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(light.direction);

    float bias = max(0.05f * (1 - dot(normal, lightDirection)), 0.005f);

    float shadow = 0;
    vec2 texelSize = 1.0f / textureSize(directionalShadowMap, 0);
    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            float pcfDepth = texture(directionalShadowMap, projected.xy + vec2(x, y) * texelSize).r;
            shadow += projected.z - bias > pcfDepth ? (1.0f / 9) : 0.0f;
        }
    }
    
    if (projected.z > 1.0f)
    {
        shadow = 0.0f;
    }

    return shadow;
}

vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor)
{
    vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;
    
    float diffuseFactor = max(0.0f, dot(normalize(Normal), normalize(direction)));
    vec4 diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;

    vec4 specularColor = vec4(0);

    if (diffuseFactor > 0.0f)
    {
        vec3 fragToCamera = normalize(cameraPos - FragPos);
        vec3 reflectedRay = normalize(reflect(direction, normalize(Normal)));

        float specularFactor = dot(fragToCamera, reflectedRay);
        if (specularFactor > 0.0f)
        {
            specularFactor = pow(specularFactor, material.shininess);
            specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
        }
    }

    return ambientColor + (1.0f - shadowFactor) * (diffuseColor + specularColor);
}

vec4 CalcDirectionalLight()
{
    float shadowFactor = CalcDirectionalShadowFactor(directionalLight);
    return CalcLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}

vec4 CalcPointLight(PointLight light, int shadowIndex)
{
    vec3 direction = FragPos - light.position;
    float distance = length(direction);
    direction = normalize(direction);

    float shadowFactor = CalcOmniDirectionalShadowFactor(light, shadowIndex);

    vec4 color = CalcLightByDirection(light.base, direction, shadowFactor);
    
    float attenuation = Evaluate(light.attenuation, distance);

    return color / attenuation;
}

vec4 CalcSpotLight(SpotLight light, int shadowIndex)
{
    vec3 rayDirection = normalize(FragPos - light.base.position);
    float spotLightFactor = dot(rayDirection, light.direction);

    if (spotLightFactor > light.edge)
    {
        vec4 color = CalcPointLight(light.base, shadowIndex);

        return color * (1.0f - (1.0f - spotLightFactor) * (1.0f / (1.0f - light.edge)));
    }
    else
    {
        return vec4(0);
    }
}

vec4 CalcSpotLights()
{
    vec4 color = vec4(0);

    for (int i = 0; i < spotLightCount; i++)
    {
        color += CalcSpotLight(spotLights[i], pointLightCount + i);
    }

    return color;
}

vec4 CalcPointLights()
{
    vec4 totalColor = vec4(0);
    
    for (int i = 0; i < pointLightCount; i++)
    {
        totalColor += CalcPointLight(pointLights[i], i);
    }

    return totalColor;
}

void main()
{
    vec4 finalColor = CalcDirectionalLight() + CalcPointLights() + CalcSpotLights();

    vec4 textureColor = texture(tex, TexCoord);
    color = textureColor * finalColor;
}