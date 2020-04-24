/*
 * Name: 2D Buffer to Buffer
 * Author: Ekkon Games - Robert Ciborowski
 * Date: 15/03/2017
 * Description: Project Aela's 2D Buffer to Buffer Renderer, which is made to
 *              specifically render a framebuffer's texture to another buffer.
*/

#version 410 core

// This is the input data.
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 UVPosition;

// This is the output data.
out vec2 UV;

void main(){
	gl_Position = vec4(vertexPosition.x, vertexPosition.y, vertexPosition.z, 1);
	UV = UVPosition;
}
