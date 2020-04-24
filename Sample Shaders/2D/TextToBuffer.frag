/*
 * Name: 2D Text Rendering
 * Author: Ekkon Games - Robert Ciborowski
 * Date: 15/03/2017
 * Description: Project Aela's Text Renderer, which renders text.
*/

#version 410 core

// This is the output data.
layout(location = 0) out vec4 colour;

// This is the input data.
in vec2 UV;
in vec4 textColourForFragment;
uniform bool antialiasing;

// This is the texture to be used.
uniform sampler2D quadTexture;

// This is main, which applies text colour based on a text's alpha value.
void main(){
	vec4 s = texture(quadTexture, UV);
	/*if (s.a < 0.05) {
		colour = vec4(0, 0, 0, 0);
	} if (s.a > 0.7) {
		colour = vec4(textColourForFragment.rgb, s.a);
	} else {
		colour = vec4(textColourForFragment.rgb, clamp(s.a, 0.0, 0.3));
	}*/
	
	// I know this is a dumb way of turning antialiasing off but its too annoying to implement
	// the good way (by using FT_LOAD_MONOCHROME). Maybe I'll just do that later.
	if (antialiasing) {
		if (s.a > 0.05) {
			s.a = min(s.a * 1.3, 1.0);
		}
		colour = vec4(textColourForFragment.rgb, s.a * textColourForFragment.a);
	} else {
		if (s.a > 0.98) {
			colour = vec4(textColourForFragment.rgb, textColourForFragment.a);
		} else {
			colour = vec4(0);
		}
	}
	
	// if (.a == 1) {
	//	colour = texture(quadTexture, UV);
	// } else {
	// 	colour = vec4(0, 0, 0, 0);
	// }
}
