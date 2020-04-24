/*
 * Name: Skybox Shader
 * Author: Ekkon Games - Robert Ciborowski
 * Date: June 2017
 * Description: Project Aela's skybox fragment shader.
*/

#version 410 core

out vec4 colour;
in vec3 coordinates;

uniform samplerCube skyboxTexture;

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

void main() {    
    /* colour = noise(texture(skyboxTexture, coordinates), coordinates.xy);
	colour.a = 1.0;*/
	colour = texture(skyboxTexture, coordinates);
}