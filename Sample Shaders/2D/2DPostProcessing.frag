/*
 * Name: 2D Post Processing
 * Author: Ekkon Games - Robert Ciborowski
 * Date: 20/03/2017
 * Description: Project Aela's Post Processor for the 2D Buffer, which contains effects
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

// This is main(), which applies any effects to the colour value of the rendered item.
void main(){
	colour = texture(quadTexture, UV);
}