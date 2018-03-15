#version 330

in vec2 f_textureCoord;

uniform sampler2D m_alphaMap;

void main()
{
    float alpha = texture2D(m_alphaMap, f_textureCoord).x;
    if (alpha < 0.1)
        discard;
    /*float depth = gl_FragCoord.z;
    float dx = dFdx(depth);
    float dy = dFdy(depth);
    gl_FragColor = vec4(depth, pow(depth, 2.0) + 0.25*(dx*dx + dy*dy), 0.0, 1.0);*/
}
