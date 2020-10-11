#version 330 core

struct Material {
    float ambient;
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 color;
};

uniform Material material;
uniform Light light;
uniform vec3 cameraPos;

in vec3 FragPos; // in world space
in vec3 Normal;  // in world space
in vec2 TexCoords;
out vec4 FragColor;

const float constant = 1.0f;
const float linear = 0.09f;
const float quadratic = 0.032f;

void main()
{
    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    // specular
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);

    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));    
    
    vec3 ambient  = material.ambient * light.color  * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse  = diff * light.color * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = spec * light.color * vec3(texture(material.texture_specular1, TexCoords));
    FragColor = vec4(attenuation * (ambient + diffuse + specular), 1.0);
}