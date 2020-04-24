/*
 * Name: 2D Texture Rendering
 * Author: Ekkon Games - Robert Ciborowski
 * Date: 15/03/2017
 * Description: Project Aela's Texture Renderer, which renders a 2D texture to a buffer.
*/

#version 410 core

// These is the input data.
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 UVPosition;
// layout(location = 0) uniform vec2 positionOfTextureOnScreen;

// This is the output data.
out vec2 UV;

// This is main(), which applies scaling to the texture. It also flips it since it would
// otherwise be rendered upside-down.
void main(){
	gl_Position = vec4(vertexPosition.x, 0 - vertexPosition.y, vertexPosition.z, 1);
	UV = UVPosition;
}

