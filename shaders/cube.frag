#version 410 core

out vec4 color;

in vec3 Normal;
in vec3 FragPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

void main()
{
    // Vectors
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    // Ambient
    vec3 ambient = material.ambient * light.ambient;

    // Diffuse
    float diff = clamp(dot(lightDir, norm), 0.0, 1.0);
    vec3 diffuse = diff * material.diffuse * light.diffuse;

    // Specular
    float spec = pow(clamp(dot(viewDir, reflectDir), 0.0, 1.0), 32);
    vec3 specular = spec * material.specular * light.specular;

    vec3 result = (diffuse + specular + ambient);
    color = vec4(result, 1.0f);

    // For test vectors
    //color = vec4(specular, 1.0f);
}

