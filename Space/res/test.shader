#shader vertex
#version 440 core

layout(location = 0) in vec3 position;

layout(location = 0) uniform mat4 u_projection;
layout(location = 4) uniform mat4 u_view;
layout(location = 8) uniform mat4 u_world;

void main() {
	gl_Position = u_projection * u_view * u_world * vec4(position, 1);
};

#shader fragment
#version 440 core

layout(location = 0) out vec4 color;

layout(location = 1) uniform vec4 u_color;

void main() {
	color = vec4(1, 1, 1, 1);
};