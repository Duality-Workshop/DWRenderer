out vec4 color;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

#ifdef AMBIENT_MAP
    uniform sampler2D tex_ambient;
#else
    uniform vec3 p_ambient;
#endif
#ifdef DIFFUSE_MAP
    uniform sampler2D tex_diffuse;
#else
    uniform vec3 p_diffuse;
#endif
#ifdef SPECULAR_MAP
    uniform sampler2D tex_specular;
#else
    uniform vec3 p_specular;
#endif
uniform float shininess;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Light light;

uniform vec3 viewPos;

#ifdef NORMAL_MAP
    uniform sampler2D tex_normal;
#endif

Material material;

void configureParamMaterial() {
#ifdef AMBIENT_MAP
    material.ambient = vec3(texture(tex_ambient, TexCoords));
#else
    material.ambient = p_ambient;
#endif
#ifdef DIFFUSE_MAP
    material.diffuse = vec3(texture(tex_diffuse, TexCoords));
#else
    material.diffuse = p_diffuse;
#endif
#ifdef SPECULAR_MAP
    material.specular = vec3(texture(tex_specular, TexCoords));
#else
    material.specular = p_specular;
#endif
    material.shininess = shininess;
}

void main()
{
    configureParamMaterial();
    // Vectors
#ifdef NORMAL_MAP
    vec3 norm = texture(normalMap, TexCoords).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    // norm = normalize(TBN * norm);
#else
    vec3 norm = normalize(Normal);
#endif
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

