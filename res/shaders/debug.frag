#version 330

out vec4 OutColor;

in vec4 f_vertexColor;

void main() {
    OutColor = f_vertexColor;
}
