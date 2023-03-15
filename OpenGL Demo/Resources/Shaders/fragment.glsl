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

float Evaluate(Quadratic quadratic, float x)
{
    return quadratic.quadratic * x * x +
        quadratic.linear * x +
        quadratic.constant;
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

vec4 CalcPointLight(PointLight light)
{
    vec3 direction = FragPos - light.position;
    float distance = length(direction);
    direction = normalize(direction);

    vec4 color = CalcLightByDirection(light.base, direction, 0.0f);
    
    float attenuation = Evaluate(light.attenuation, distance);

    return color / attenuation;
}

vec4 CalcSpotLight(SpotLight light)
{
    vec3 rayDirection = normalize(FragPos - light.base.position);
    float spotLightFactor = dot(rayDirection, light.direction);

    if (spotLightFactor > light.edge)
    {
        vec4 color = CalcPointLight(light.base);

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
        color += CalcSpotLight(spotLights[i]);
    }

    return color;
}

vec4 CalcPointLights()
{
    vec4 totalColor = vec4(0);
    
    for (int i = 0; i < pointLightCount; i++)
    {
        totalColor += CalcPointLight(pointLights[i]);
    }

    return totalColor;
}

void main()
{
    vec4 finalColor = CalcDirectionalLight() + CalcPointLights() + CalcSpotLights();

    vec4 textureColor = texture(tex, TexCoord);
    color = textureColor * finalColor;
}