#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos; // Position var | Attribute pos 0

out vec4 vertexColour;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	// Multiplication read from right to left
	gl_Position = projection * view * model * vec4(aPos, 1.0); // vec3 given directly to vec4 constructor
	vertexColour = vec4(0.5f, 0.0f, 0.0f, 1.0f);
};

#shader fragment
#version 330 core

out vec4 fragColour;
in vec4 vertexColour; // Get colour from vertex shader

void main() {
	fragColour = vertexColour;
};