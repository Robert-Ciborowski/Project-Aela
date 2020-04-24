/*
 * Name: Billboard Shader
 * Author: Ekkon Games - Robert Ciborowski
 * Date: November 2016
 * Description: Project Aela's billboard fragment shader.
*/

#version 410 core

// This is the output data.
layout(location = 0) out vec4 colour;

in vec2 UV;

// Values that stay constant for the whole mesh.
uniform sampler2D textureSampler;

// This is used for pseudo-randomness.
float rand(vec2 seed){
    return fract(sin(dot(seed.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

// This is for noise. This effect can be found in the 3D post-process shader. I put it here
// in case one might want to apply the effect to an individual texture rather than to the
// entire 3D buffer.
float noiseMagnitude = 0.07;
vec4 noise(vec4 colourToModify, vec2 UV) {
	return clamp(vec4(colourToModify + vec4(rand(UV) * noiseMagnitude) - vec4(noiseMagnitude / 2)), 0, 1);
}

void main(){
	// colour = noise(texture(textureSampler, UV), UV);
	colour = texture(textureSampler, UV);
}