#version 330 core

out vec4 fragColour;
in vec4 vertexColour; // Get colour from vertex shader

uniform vec3 lightColour;
uniform vec3 objectColour;

void main() {
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColour;
	vec3 result = ambient * objectColour;

	fragColour = vec4(result, 1.0);
};