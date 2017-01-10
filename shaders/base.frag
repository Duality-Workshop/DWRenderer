layout (location=0) out vec4 color;
layout (location=1) out vec4 brightColor;

// TODO : deffered-shading

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

uniform Light lights[2];

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
	vec3 lightning = vec3(0.0f);
	for(int i = 0; i < 2; ++i) {
		vec3 lightDir = normalize(lights[i].position - FragPos);
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);

		// Ambient
		vec3 ambient = material.ambient * lights[i].ambient;

		// Diffuse
		float diff = clamp(dot(lightDir, norm), 0.0, 1.0);
		vec3 diffuse = diff * material.diffuse * lights[i].diffuse;

		// Specular
		float spec = pow(clamp(dot(viewDir, reflectDir), 0.0, 1.0), 32);
		vec3 specular = spec * material.specular * lights[i].specular;

		vec3 result = (diffuse + specular + ambient);
		
		// Attenuation (use quadratic as we have gamma correction)
        float dist = length(FragPos - lights[i].position);
		result *= 1.0f / (dist * dist);
		lightning += result;
	}

	// Check whether result is higher than some threshold, if so, output as bloom threshold color
    float brightness = dot(lightning, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        brightColor = vec4(lightning, 1.0);
    else
        brightColor = vec4(vec3(0.0), 1.0);
    color = vec4(lightning, 1.0f);

    // For test vectors
    //color = vec4(specular, 1.0f);
	//color = vec4(material.diffuse, 1.0f);
	//color = vec4(light.diffuse, 1.0f);
}

