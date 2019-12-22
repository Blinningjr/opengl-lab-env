#version 310 es

precision mediump float;

layout(location=0) in vec3 pos;
layout(location=0) out vec4 Color;

uniform mat4 camera;
uniform mat4 object_transform;
uniform vec3 color;

void main() {
	gl_Position = (camera * object_transform * vec4(pos, 1));
	Color = vec4(color, 1);
}
