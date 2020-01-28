#version 330 core

precision mediump float;

in vec3 pos;
in vec3 normal;

out vec3 FragPos; 
out vec3 Normal;

uniform mat4 projection;
uniform mat4 camera;
uniform mat4 object_transform;

void main() {
	gl_Position = (projection * camera * object_transform * vec4(pos, 1));
	FragPos = vec3(object_transform * vec4(pos, 1));
	Normal = mat3(transpose(inverse(object_transform))) * normal;  
}
