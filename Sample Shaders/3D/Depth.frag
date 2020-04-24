/*
 * Name: Depth Shader
 * Author: Ekkon Games - Robert Ciborowski
 * Date: October 2016
 * Description: Project Aela's depth fragment shader.
*/

#version 430 core

// This is the fragment position, which is obtained from the geometry shader.
in vec4 fragmentPosition;

// This is the position of the light.
uniform vec3 lightPosition;

// This stores the maximum distance that the shadow map allows.
float far = 100.0;

// This is main(), which calculates colour using depth.
void main(){
    float lightDistance = length(fragmentPosition.xyz - lightPosition);
    lightDistance = lightDistance / far;
    gl_FragDepth = lightDistance;
}