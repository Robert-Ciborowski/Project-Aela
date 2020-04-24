/*
 * Name: Model Shader
 * Author: Ekkon Games - Robert Ciborowski
 * Date: October 2016
 * Description: Project Aela's modelMatrices[gl_InstanceID] vertex shader.
*/

#version 410 core

// This is the input data.
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;

// This is the output data.
out vec2 UV;
out vec3 worldSpacePosition;
// out vec3 modelViewProjectionPosition;
// out vec3 cameraSpaceNormal;
// out vec3 cameraSpaceEyeDirection;
out vec3 normal;

// These are values that stay constant for the whole mesh.
uniform mat4 viewMatrix;
uniform mat4 modelMatrices[100];
uniform mat4 rotationMatrices[100];
uniform mat4 projectionMatrix;

void main(){
	mat4 modelViewMatrix = viewMatrix * modelMatrices[gl_InstanceID];

	// This is the output position transformed by the model, view and projection matrices.
	gl_Position = projectionMatrix * modelViewMatrix * vec4(vertexPosition, 1);
	
	// modelViewProjectionPosition = vertexPosition;
	
	normal = vec3(rotationMatrices[gl_InstanceID] * vec4(vertexNormal, 1));
	
	// This is the position of the vertex in the world space.
	worldSpacePosition = vec3(modelMatrices[gl_InstanceID] * vec4(vertexPosition, 1.0));
	
	// This is a vector that goes from the vertex to the camera.
	// cameraSpaceEyeDirection = vec3(0,0,0) - (modelViewMatrix * vec4(vertexPosition,1)).xyz;
	
	// This calculates the normal of the vertex.
	// cameraSpaceNormal = (modelViewMatrix * vec4(vertexNormal,0)).xyz;
	
	UV = vertexUV;
}

