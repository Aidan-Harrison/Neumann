#version 330 core
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float roughness;
};
uniform Material material; // Create object uniform

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;

out vec4 fragColour;
in vec4 vertexColour; // Get colour from vertex shader
in vec3 normal;
in vec3 fragPos;

uniform vec3 lightColour;
uniform vec3 objectColour;
uniform vec3 lightPos;
uniform vec3 viewPos;

	// float ambientStrength = 0.2f, specularStrength = 0.1f; // Possibly keep ambient at 0.1f
	// vec3 ambient = ambientStrength * lightColour;

void main() {
		// ambient
		// Diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
		// Specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm); // Built in reflect(lightSource, fragment) function
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.roughness); // Roughness defines highlight size

	vec3 ambient = light.ambient * material.ambient;
	vec3 diffuse = light.diffuse * (diff * material.diffuse);
	vec3 specular = light.specular * (spec * material.specular);

	vec3 result = ambient + diffuse + specular;
	fragColour = vec4(result, 1.0);
};