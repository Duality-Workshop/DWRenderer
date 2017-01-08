#version 450

out vec4 color;
in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform sampler2D bloomBlurBuffer;
uniform float exposure;
uniform bool hdr;
uniform bool bloom;

void main()
{             
    const float gamma = 2.2;
    
	vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
	//vec3 hdrColor = texelFetch(hdrBuffer, ivec2(gl_FragCoord.xy), 0).rgb;
	
	if (hdr && bloom) {
		vec3 bloomColor = texture(bloomBlurBuffer, TexCoords).rgb;
		hdrColor += bloomColor; // additive blending
	}

	// TODO : Tonemapper : Work on the luminance of hdrColor then use Reinhard's formula

    // reinhard basic
    //vec3 result = hdrColor / (hdrColor + vec3(1.0));
    // exposure
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // also gamma correct while we're at it       
    result = pow(result, vec3(1.0 / gamma));
	if (hdr)
		color = vec4(result, 1.0f);
	else
		color = vec4(hdrColor, 1.0f);
}