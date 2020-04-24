/*
 * Name: Model Shader Without Lights
 * Author: Ekkon Games - Robert Ciborowski
 * Date: October 2016
 * Description: Project Aela's model fragment shader, which does not use Aela's lighting system.
 *              The only source of light is the ambient lighting.
*/

#version 410 core

// This is the input data.
in vec2 UV;
in vec3 worldSpacePosition;
// in vec3 modelViewProjectionPosition;
// in vec3 cameraSpaceNormal;
// in vec3 cameraSpaceEyeDirection;
in vec3 normal;

// This is the output data.
vec3 colourAsVec3;
layout(location = 0) out vec4 colour;

// These are the uniforms used by the shader.
uniform sampler2D textureSampler;
uniform vec3 globalLighting;

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
	// This calculates several colours.
	vec4 UVSample = texture(textureSampler, UV).rgba;
	vec3 materialDiffuseColor = UVSample.rgb;
	
	// colour = noise(vec4(materialDiffuseColor * globalLighting, 1), UV);
	colour = vec4(materialDiffuseColor * globalLighting, UVSample.a);
	// colour = vec4(1, 0.5, 0.8, 1);
	// colour = UVSample;
	// colour = vec4(1);
}