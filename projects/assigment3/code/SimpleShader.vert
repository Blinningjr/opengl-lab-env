#version 310 es

precision mediump float;

layout(location=0) in vec3 pos;
layout(location=0) out vec4 Color;

uniform vec3 light;
uniform mat4 camera;
uniform mat4 object_transform;
uniform vec3 color;

void main() {
	gl_Position = (camera * object_transform * vec4(pos, 1));
	float dist = sqrt(pow(pos.x - light.x, 2.0) + pow(pos.y - light.y, 2.0) + pow( pos.z - light.z, 2.0));
	Color = (vec4(color, 1) - dist);
}
