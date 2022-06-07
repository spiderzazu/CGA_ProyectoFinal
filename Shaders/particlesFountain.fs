#version 400
in float Transp;
in float visibility;
uniform sampler2D ParticleTex;

uniform vec3 fogColor;

layout ( location = 0 ) out vec4 FragColor;

void main()
{
    FragColor = texture(ParticleTex, gl_PointCoord);
    FragColor.a *= Transp;
}