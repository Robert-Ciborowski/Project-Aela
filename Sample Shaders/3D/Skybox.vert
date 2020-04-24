/*
 * Name: Skybox Shader
 * Author: Ekkon Games - Robert Ciborowski
 * Date: June 2017
 * Description: Project Aela's skybox vertex shader.
*/

#version 410 core

// This is the input data. It's self explanatory.
layout (location = 0) in vec3 vertexPosition;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 coordinates;

void main() {
    coordinates = vertexPosition;
	vec4 position2 = projectionMatrix * viewMatrix * vec4(vertexPosition, 1.0);
    gl_Position = position2.xyww;
}  