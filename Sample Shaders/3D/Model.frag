/*
 * Name: Model Shader
 * Author: Ekkon Games - Robert Ciborowski
 * Date: October 2016
 * Description: Project Aela's model fragment shader.
*/

#version 430 core

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

// This represents the max number of lights allowed.
const int MAX_LIGHT_AMOUNT = 5;

// This represents how bright an object can get as it gets closer to a light source.
const float MAX_DISTANCE_MODIFIER_RESULT = 1.5;

// This modifies the minimum brightness as the angle changes. It larger this value is
// the less the light decays as you go further away from it.
const float COS_THETA_MINIMUM = 0.4;
const float COS_THETA_MAXIMUM = 0.8;

float distanceToLightModifier = 0.065; // 0.05
bool PCF = true;
float PI  = 3.14159265358979323846;
float far = 100.0;
float shadowScalingFactor = 1;
vec3 PCFDirections[20] = vec3[](
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

// These are the uniforms used by the shader.
// I have no clue as to why, but openGL does not return the proper uniform address unless I name these
// uniforms EXACTLY like this.
// uniform float ambientLighting;
uniform vec3 ambientLighting;
uniform sampler2D textureSampler;
uniform samplerCube shadowMaps[MAX_LIGHT_AMOUNT];
uniform int numberOfLights;
uniform vec3 openGLSucksAtPositions[MAX_LIGHT_AMOUNT];
// uniform vec3 lightDirections[MAX_LIGHT_AMOUNT];
uniform vec3 openGLSucksAtColours[MAX_LIGHT_AMOUNT];
uniform float openGLSucksAtLightPowers[MAX_LIGHT_AMOUNT];

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

// This calculates the shadow. It contains a few methods of making shadows look nicer,
// including the fixing of shadow acne and peter panning. It also includes percentage-closer
// filtering (PCF).
float shadowCalculation(vec3 positionInLightSpace, int whichLight, float bias) {
    vec3 fragToLight = positionInLightSpace - openGLSucksAtPositions[whichLight];
    float currentDepth = length(fragToLight) * shadowScalingFactor;
	if (currentDepth > far) {
		return 0;
	}

	float shadow = 0;
	
	// This undergoes the PCF process, if enabled.
	if (PCF) {
		float PCFRadius = 0.03;
		for (int i = 0; i < PCFDirections.length(); i++) {
			float closestDepth = texture(shadowMaps[whichLight], fragToLight + PCFDirections[i] * PCFRadius).r;
			closestDepth *= far * shadowScalingFactor;
			if (currentDepth - bias > closestDepth) {
				shadow += 1.0;
			}
		}
		shadow /= float(PCFDirections.length());
	} else {
		float closestDepth = texture(shadowMaps[whichLight], fragToLight).z;
		closestDepth *= far * shadowScalingFactor;
		shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
	}
	
	// return closestDepth / far;
    return shadow;
}  

void main(){
	// This calculates several colours.
	vec4 UVSample = texture(textureSampler, UV).rgba;
	vec3 materialDiffuseColor = UVSample.rgb;
	vec3 MaterialSpecularColor = vec3(0.3, 0.3, 0.3);
	vec3 diffuseColours[MAX_LIGHT_AMOUNT];
	vec3 finalDiffuseColour = vec3(0, 0, 0);
	
	float visibility = 1.0;
	
	for (int i = 0; i < numberOfLights; i++) {
		vec3 n = normal;
		vec3 l = normalize(openGLSucksAtPositions[i] - worldSpacePosition);
		
		// This calculates the cosine between the normal and the light direction. It is clamped.
		// If the light is at the vertical of the triangle, cosTheta is 1.
		// If the light perpendicular to the triangle, cosTheta is 0.
		// If the light behind the triangle, cosTheta is 0.
		float cosTheta = clamp(dot(n, l), COS_THETA_MINIMUM, COS_THETA_MAXIMUM);
		
		float bias = 0.005 * tan(acos(cosTheta));
		bias = clamp(bias, 0.0, 0.01);
		
		float shadow = shadowCalculation(worldSpacePosition, i, bias);
		visibility -= shadow;
		
		float distanceBetweenLightAndFragment = distance(worldSpacePosition, openGLSucksAtPositions[i]);
		float distanceModifier = clamp(((1 / distanceBetweenLightAndFragment) / distanceToLightModifier), 0, MAX_DISTANCE_MODIFIER_RESULT);
		diffuseColours[i] = materialDiffuseColor * openGLSucksAtColours[i] * openGLSucksAtLightPowers[i] * cosTheta * distanceModifier;
		visibility += cosTheta * openGLSucksAtLightPowers[i];
	}
	
	for (int i = 0; i < numberOfLights; i++) {
		finalDiffuseColour += diffuseColours[i];
	}
	
	finalDiffuseColour = clamp(finalDiffuseColour, 0.0, 1.0);
	visibility = clamp(visibility, 0.0, 1.0);
	
	colourAsVec3 = ambientLighting * materialDiffuseColor + visibility * finalDiffuseColour;
	// colour = noise(vec4(colourAsVec3, 1), UV);
	colour = vec4(colourAsVec3, UVSample.a);
	// colour = vec4(0.5, 1, 0.5, 1);
	// colour = UVSample;
}