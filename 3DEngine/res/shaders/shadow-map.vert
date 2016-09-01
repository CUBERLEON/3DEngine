#version 330

out vec2 f_textureCoord;

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_textureCoord;

uniform mat4 o_vpLightTransform;
uniform mat4 t_mTransform;

void main()
{
	f_textureCoord = v_textureCoord;
	gl_Position = o_vpLightTransform * t_mTransform * vec4(v_position, 1.0);
}
