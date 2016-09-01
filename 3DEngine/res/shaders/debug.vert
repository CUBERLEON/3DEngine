#version 330

out vec3 g_normal;
out vec3 g_tangent;
out vec3 g_bitangent;

layout (location = 0) in vec3 v_position;
layout (location = 2) in vec3 v_normal;
layout (location = 3) in vec3 v_tangent;

//transform
uniform mat4 t_mvpTransform;

void main() {
	float len = 0.5;
	g_normal = v_normal * len;
	g_tangent = v_tangent * len;
	//g_bitangent = cross(v_normal, v_tangent) * len;
	g_bitangent = cross(v_tangent, v_normal) * len;
	gl_Position = vec4(v_position, 1.0);
}
