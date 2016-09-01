#version 330

layout (line_strip, max_vertices = 18) out;
out vec4 f_vertexColor;

layout (triangles) in;

in vec3 g_normal[];
in vec3 g_tangent[];
in vec3 g_bitangent[];

//transform
uniform mat4 t_mvpTransform;

void main()
{
	for (int i = 0; i < 3; ++i)
	{
		vec3 pos = gl_in[i].gl_Position.xyz;
		vec3 normal = g_normal[i];
		vec3 tangent = g_tangent[i];
		vec3 bitangent = g_bitangent[i];
    
		//normal
		gl_Position = t_mvpTransform * vec4(pos, 1.0);
		f_vertexColor = vec4(1, 0, 0, 1);
		EmitVertex();    
		gl_Position = t_mvpTransform * vec4(pos + normal, 1.0);
		f_vertexColor = vec4(1, 0, 0, 1);
		EmitVertex();
		EndPrimitive();

		/*//tangent
		gl_Position = t_mvpTransform * vec4(pos, 1.0);
		f_vertexColor = vec4(0, 1, 0, 1);
		EmitVertex();    
		gl_Position = t_mvpTransform * vec4(pos + tangent, 1.0);
		f_vertexColor = vec4(0, 1, 0, 1);
		EmitVertex();
		EndPrimitive();

		//bitangent
		gl_Position = t_mvpTransform * vec4(pos, 1.0);
		f_vertexColor = vec4(0, 0, 1, 1);
		EmitVertex();    
		gl_Position = t_mvpTransform * vec4(pos + bitangent, 1.0);
		f_vertexColor = vec4(0, 0, 1, 1);
		EmitVertex();
		EndPrimitive();*/
	}
}