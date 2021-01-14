#version 330 core

struct Material {
	sampler2D texDiffuse;
	sampler2D texSpecular;

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

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 pAmbient;
	vec3 pDiffuse;
	vec3 pSpecular;
};
uniform PointLight pLights[4]; // Have to use defines if using variables

out vec4 fragColour;
in vec4 vertexColour; // Get colour from vertex shader
in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

uniform vec3 lightColour;
uniform vec3 objectColour;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D ourTexture;

	// float ambientStrength = 0.2f, specularStrength = 0.1f; // Possibly keep ambient at 0.1f
	// vec3 ambient = ambientStrength * lightColour;

// Other Lights
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);
		// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
		// Specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.roughness);
		// Combine results | Check!
	vec3 ambient = light.ambient * material.diffuse;
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 specular = light.specular * spec * material.specular;
	return(ambient + diffuse + specular);
}

// Loop through this for multiple point lights
//vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	//vec3 lightDir = normalize(light.position - fragPos);
		// Diffuse
	//float diff = max(dot(normal, lightDir), 0.0);
		// Specular
	//vec3 reflectDir = reflect(-lightDir, normal);
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.roughness);
		// Attenuation
	//float distance = length(light.position - fragPos);
	//float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
		// Combine results | Check!
	//vec3 ambient = light.pAmbient * vec3(texure(material.texDiffuse, texCoords));
	//vec3 diffuse = light.pDiffuse * diff * vec3(texture(material.texDiffuse, texCoords));
	//vec3 specular = light.pSpecular * spec * vec3(texture(material.texSpecular, texCoords));
	//ambient *= attenuation;
	//diffuse *= attenuation;
	//specular *= attenuation;
	//return(ambient + diffuse + specular);
//}

void main() {
	// Regular
		// Diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
		// Specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm); // Built in reflect(lightSource, fragment) function
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.roughness); // Roughness defines highlight size

	vec3 ambient = light.ambient * vec3(texture(material.texDiffuse, texCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texDiffuse, texCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.texSpecular, texCoords));

	vec3 result = ambient + diffuse + specular;
	fragColour = vec4(result, 1.0);
	// fragColour = texture(ourTexture, texCoords);

	// Multi light version
	//vec3 norm = normalize(normal);
	//vec3 viewDir = normalize(viewPos - fragPos);
		// Directional	 
	//vec3 result = CalcDirLight(dirLight, norm, viewDir);
		// Point
	// for(unsigned int i = 0; i < 4; i++)
		// result += CalcPointLight(pLights[1], norm, fragPos, viewDir);
	//fragColour = vec4(result, 1.0);
};