/*
 * Name: 2D Post Processing
 * Author: Ekkon Games - Robert Ciborowski
 * Date: 20/03/2017
 * Description: Project Aela's Post Processor for the 2D Buffer, which contains effects
                as well as scaling.
*/

#version 410 core

// This is the input data.
layout(location = 0) in vec3 vertexPosition;
uniform vec2 positionOfTextureOnScreen;
uniform vec2 windowDimensions;

// This is the output data.
out vec2 UV;
out vec2 windowDimensionsForFragmentShader;

float convertFromPositionToUV(float value) {
	return (value + 1) / 2;
}

// This is main(), which calculates position and UV data based on the scaling requirements given.
void main(){
	gl_Position = vec4(vertexPosition.x, vertexPosition.y, vertexPosition.z, 1);
	UV = vec2(convertFromPositionToUV(vertexPosition.x) - positionOfTextureOnScreen.x, convertFromPositionToUV(vertexPosition.y) - positionOfTextureOnScreen.y);
	windowDimensionsForFragmentShader = windowDimensions;
}

