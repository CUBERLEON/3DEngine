#version 330

out vec2 f_textureCoord;
out vec3 f_normal;
out vec3 f_worldPosition;
out mat3 f_TBN;

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_textureCoord;
layout (location = 2) in vec3 v_normal;
layout (location = 3) in vec3 v_tangent;

//transform
uniform mat4 t_mTransform;
uniform mat4 t_mvpTransform;

void main() {
    f_textureCoord = v_textureCoord;
    f_normal = normalize(t_mTransform * vec4(v_normal, 0)).xyz;
    f_worldPosition = (t_mTransform * vec4(v_position, 1)).xyz;

    vec3 N = normalize((t_mTransform * vec4(v_normal, 0)).xyz);
    vec3 T = normalize((t_mTransform * vec4(v_tangent, 0)).xyz);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(T, N);
    f_TBN = mat3(T, B, N);

    gl_Position = t_mvpTransform * vec4(v_position, 1.0);
}
