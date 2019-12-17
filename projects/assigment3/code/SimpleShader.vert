#version 310 es

precision mediump float;

layout(location=0) in vec3 pos;
layout(location=0) out vec4 Color;

uniform vec3 objTransform;
uniform vec3 color;

void main() {
	gl_Position = vec4(objTransform * pos, 1);
	Color = vec4(color, 1);
}
