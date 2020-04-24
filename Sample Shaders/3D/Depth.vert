/*
 * Name: Depth Shader
 * Author: Ekkon Games - Robert Ciborowski
 * Date: October 2016
 * Description: Project Aela's depth vertex shader.
*/

#version 410 core

// This is the input data.
layout(location = 0) in vec3 vertexPosition;

// These are the uniforms.
uniform mat4 modelMatrices[100];

void main(){
	gl_Position = modelMatrices[gl_InstanceID] * vec4(vertexPosition, 1);
}

