#version 330 core

out vec4 fragColour;
in vec4 vertexColour; // Get colour from vertex shader
in vec3 normal;
in vec3 fragPos;

uniform vec3 lightColour;
uniform vec3 objectColour;
uniform vec3 lightPos;
uniform vec3 viewPos;


void main() {
		// ambient
	float ambientStrength = 0.2f, specularStrength = 0.1f; // Possibly keep ambient at 0.1f
	vec3 ambient = ambientStrength * lightColour;
		// Diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColour;
		// Specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm); // Built in reflect(lightSource, fragment) function
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // 32 = Highlight size
	vec3 specular = specularStrength * spec * lightColour;

	vec3 result = (ambient + diffuse + specular) * objectColour;
	fragColour = vec4(result, 1.0);
};