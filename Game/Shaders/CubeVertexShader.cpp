#version 460 core

layout(location = 0) in vec3 in_vertex_pos;
layout(location = 1) in vec3 in_vertex_color;

uniform mat4 view_projection;
uniform mat4 model_matrix;

out vec3 fragment_color;

void main() {

	// This isn't numbers so A * B != B * A...
	gl_Position = view_projection * model_matrix * vec4(in_vertex_pos, 1);
	fragment_color = in_vertex_color;
}