/*
 * Name: 3D Post Processing
 * Author: Ekkon Games - Robert Ciborowski
 * Date: 20/03/2017
 * Description: Project Aela's Post Processor for the 3D Buffer, which contains effects
                as well as scaling.
*/

#version 410 core

// This is the output data.
layout(location = 0) out vec4 colour;

// This is the texture to be used.
uniform sampler2D quadTexture;

// This is the input data.
in vec2 UV;
in vec2 windowDimensionsForFragmentShader;

// This is used for transparency. Keep in mind that making the 3D buffer transparent
// will make any buffers behind it more visible.
float transparency = 1.0;

// This stores the effects to use.
int effectsToUse[1] = int[1](0);

// These functions and variables are the effects that may be used on the buffer.
// This is the blur effect.
vec4 blur(vec4 colourToModify);
float blurMagnitude = 1;

// This is the black and white effect. If it is too dark or light, use the
// brightness modifier.
vec4 blackAndWhite(vec4 colourToModify);
float brightnessModifier = 0.05;

// This is the tint effect.
vec4 tint(vec4 colourToModify);
// Tint Colour: 0 = tint to black, 1.0 = keep original value.
vec3 tintColour = vec3(0.99, 0.91, 0.84);

// This is for noise.
vec4 noise(vec4 colourToModify, vec2 UV);
// This is a modifier for the amplitude of the noise.
float noiseMagnitude = 0.05;

// This is used for pseudo-randomness.
float rand(vec2 seed){
    return fract(sin(dot(seed.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

// This is main(), which applies any effects to the colour value of the rendered item.
void main(){
	colour = texture(quadTexture, UV);
	transparency = colour.a;
	for (int i = 0; i < effectsToUse.length(); i++) {
		switch (effectsToUse[i]) {
			case 0:
				colour = texture(quadTexture, UV);
				colour.a = transparency;
				break;
			case 1:
				colour = blur(colour);
				break;
			case 2:
				colour = blackAndWhite(colour);
				break;
			case 3:
				colour = tint(colour);
				break;
			case 4:
				colour = noise(colour, UV);
				break;
			default:
				colour = texture(quadTexture, UV);
				colour.a = transparency;
				break;
		}
	}
}

// These are the defined functions of the effects.
vec4 blur(vec4 colourToModify){
	float changeInX = (1.0 / windowDimensionsForFragmentShader.x);
	float changeInY = (1.0 / windowDimensionsForFragmentShader.y);
	
	for (float i = changeInX * -blurMagnitude; i < changeInX * blurMagnitude; i += changeInX) {
		for(float j = changeInY * -blurMagnitude; j < changeInY * blurMagnitude; j += changeInY) {
			colourToModify += texture(quadTexture, vec2(UV.x + i, UV.y + j));
		}
	}
	
	colourToModify /= 4 * blurMagnitude * blurMagnitude;
	return colourToModify;
}

vec4 blackAndWhite(vec4 colourToModify){
	float amount = (colourToModify.x + colourToModify.y + colourToModify.z) / 3 + brightnessModifier;	
	colourToModify = vec4(amount, amount, amount, transparency);
	return colourToModify;
}

vec4 tint(vec4 colourToModify) {
	return colourToModify * vec4(tintColour, 1);
}

vec4 noise(vec4 colourToModify, vec2 UV) {
	return clamp(vec4(colourToModify + vec4(rand(UV) * noiseMagnitude) - vec4(noiseMagnitude / 2)), 0, 1);
}