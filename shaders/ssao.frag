#version 450

out float color;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D texNoise;
uniform vec3 samples[64]; // TODO : dynamic nbSamples
uniform mat4 projection;
uniform vec2 noiseScale;

// Parameters of SSAO
int kernelSize = 64; // nbSamples
float radius = 0.5f;
float bias = 0.025f;

void main()
{
	vec3 randomVec = texture(texNoise, TexCoords * noiseScale).xyz;
	
	vec3 tangent   = normalize(randomVec - Normal * dot(randomVec, Normal));
	vec3 bitangent = cross(Normal, tangent);
	mat3 TBN       = mat3(tangent, bitangent, Normal);

	float occlusion = 0.0;
	for(int i = 0; i < kernelSize; ++i) {
		// Get sample position
		vec3 samp = TBN * samples[i]; // From tangent to view-space
		samp = FragPos + samp * radius;
		
		// project sample position (to sample texture) (to get position on screen/texture)
        //vec4 offset = vec4(samp, 1.0);
        //offset = projection * offset; // from view to clip-space
        //offset.xyz /= offset.w; // perspective divide
        //offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        
        // get sample depth
        float sampleDepth = FragPos.z; // Get depth value of kernel sample
        
        // range check & accumulate
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(FragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samp.z + bias ? 1.0 : 0.0) * rangeCheck;           
    }
    occlusion = 1.0 - (occlusion / kernelSize);
    
	color = occlusion;
}