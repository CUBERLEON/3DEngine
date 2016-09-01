#version 330

out vec4 OutColor;

in vec3 f_skyboxCoord;

uniform samplerCube o_skybox;

void main()
{
	OutColor = texture(o_skybox, f_skyboxCoord);
}