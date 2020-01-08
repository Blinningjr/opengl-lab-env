#version 310 es

precision mediump float;

layout(location=0) in vec3 pos;
layout(location=0) out vec4 Color;

uniform vec3 light;
uniform mat4 projection;
uniform mat4 camera;
uniform mat4 object_transform;
uniform vec3 color;

void main() {
	gl_Position = (projection * camera * object_transform * vec4(pos, 1));
	float fragPos = vec3(object_transform * vec4(pos, 1.0));
	float dist = sqrt(pow(fragPos.x - light.x, 2.0) + pow(fragPos.y - light.y, 2.0) + pow(fragPos.z - light.z, 2.0));
	Color = (vec4(color, 1) - (dist * 1.0));
}
