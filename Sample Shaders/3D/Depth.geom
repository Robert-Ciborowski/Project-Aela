/*
 * Name: Depth Shader
 * Author: Ekkon Games - Robert Ciborowski
 * Date: June 2017
 * Description: Project Aela's depth geometry shader.
*/

#version 410 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

// This uniform transforms vertices to the six sides of the cubed shadow map.
uniform mat4 shadowMatrices[6];

// This outputs the position to the fragment shader.
out vec4 fragmentPosition;

void main() {
    for (int whichFace = 0; whichFace < 6; whichFace++) {
        gl_Layer = whichFace;
        for (int i = 0; i < 3; i++) {
            fragmentPosition = gl_in[i].gl_Position;
			gl_Position = shadowMatrices[whichFace] * fragmentPosition;
            EmitVertex();
        }    
        EndPrimitive();
    }
}  