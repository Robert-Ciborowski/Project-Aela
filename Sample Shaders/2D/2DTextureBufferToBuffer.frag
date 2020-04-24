/*
 * Name: 2D Buffer to Buffer
 * Author: Ekkon Games - Robert Ciborowski
 * Date: 15/03/2017
 * Description: Project Aela's 2D Buffer to Buffer Renderer, which is made to
 *              specifically render a framebuffer's texture to another buffer.
*/

#version 410 core

// This is the input data.
layout(location = 0) out vec4 colour;

// This is the texture to be used.
uniform sampler2D quadTexture;

uniform vec4 tintMultiplier = vec4(1, 1, 1, 1);

// This is the output data.
in vec2 UV;

// This is main(), which simply gets colour.
void main(){
	colour = texture(quadTexture, UV) * tintMultiplier;
}
