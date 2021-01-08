#version 330 core

layout(location = 0) in vec3 aPos; // Position var | Attribute pos 0
layout(location = 1) in vec3 aNormal; // Normal vector

out vec4 vertexColour;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
		// Multiplication read from right to left
	gl_Position = projection * view * model * vec4(aPos, 1.0); // vec3 given directly to vec4 constructor
	vertexColour = vec4(0.5, 0.0, 0.0, 1.0);
}