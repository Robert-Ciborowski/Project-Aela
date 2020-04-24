/*
 * Name: Billboard Shader
 * Author: Ekkon Games - Robert Ciborowski
 * Date: November 2016
 * Description: Project Aela's billboard vertex shader.
*/

#version 410 core

// This is the input data.
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;

// This is the output data.
out vec2 UV;

uniform mat4 modelViewProjectionMatrix;

void main(){
	gl_Position =  modelViewProjectionMatrix * vec4(vertexPosition, 1);
	UV = vertexUV;
}