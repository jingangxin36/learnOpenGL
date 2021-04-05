#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 ViewDir;
out vec3 FragPos;
uniform mat4 trans;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0));
	ViewDir = normalize(viewPos - FragPos);
	Normal = mat3(transpose(inverse(model))) * aNormal;
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 ViewDir;
in vec3 FragPos;

uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;
uniform float mixValue;
uniform vec4 objectColor;
uniform vec3 lightColor;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;

void main()
{
	vec3 lightDir = normalize(light.position - FragPos);

	vec3 ambient = light.ambient * material.ambient;

	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	float specularStrength = 0.7;
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(ViewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}