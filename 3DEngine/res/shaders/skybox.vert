#version 330

out vec3 f_skyboxCoord;

layout (location = 0) in vec3 v_position;

//other
uniform mat4 o_skyboxTransform;

void main()
{
	gl_Position = (o_skyboxTransform * vec4(v_position, 1.0)).xyww;
	f_skyboxCoord = v_position;
}
