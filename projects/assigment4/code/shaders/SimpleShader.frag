#version 330 core

precision mediump float;

in vec3 FragPos; 
in vec3 Normal;

out vec4 Color;

uniform vec3 color;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;

uniform vec3 cameraPos;

void main() {
	float ambientStrength = 0.01;
    vec3 ambient = (ambientStrength * lightColor);

	vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = (diff * lightColor) * lightIntensity;

	vec3 viewDirection = normalize(cameraPos - FragPos);
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
	vec3 specular = lightIntensity * spec * lightColor;  

	Color = vec4(((ambient + diffuse+ specular) * color), 1.0);
}
