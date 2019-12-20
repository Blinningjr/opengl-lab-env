#version 310 es

precision mediump float;

layout(location=0) in vec4 color;
out vec4 Color;

void main() {
	Color = color;
}
