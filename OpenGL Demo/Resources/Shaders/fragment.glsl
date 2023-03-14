#version 330

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

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

vec4 CalcLightByDirection(Light light, vec3 direction)
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

    return (ambientColor + diffuseColor + specularColor);
}

vec4 CalcDirectionalLight()
{
    return CalcLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4 CalcPointLight(PointLight light)
{
    vec3 direction = FragPos - light.position;
    float distance = length(direction);
    direction = normalize(direction);

    vec4 color = CalcLightByDirection(light.base, direction);
    
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