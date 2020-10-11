#version 330 core

struct Material {
    float ambient;
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;
uniform vec3 cameraPos;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} fs_in;

out vec4 FragColor;

void main()
{
    // normal mapping
    vec3 normal = texture(material.texture_normal1, fs_in.TexCoords).rgb;
    normal = -(normal * 2.0 - 1.0);             // in tangent space, why is it inverted??
    //normal = vec3(0.0, 0.0, 1.0);
    normal = normalize(fs_in.TBN * normal);     // in world space

    // diffuse
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    float diff = max(dot(normal, lightDir), 0.0);

    // specular
    vec3 viewDir = normalize(cameraPos - fs_in.FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    // attenuation
    float distance    = length(light.position - fs_in.FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    vec3 ambient  = material.ambient * light.color  * vec3(texture(material.texture_diffuse1, fs_in.TexCoords));
    vec3 diffuse  = diff * light.color * vec3(texture(material.texture_diffuse1, fs_in.TexCoords));
    vec3 specular = spec * light.color * vec3(texture(material.texture_specular1, fs_in.TexCoords));
    FragColor = vec4(attenuation * (ambient + diffuse + specular), 1.0);
    //FragColor = vec4(normal / 2.0 +0.5, 1.0);
}