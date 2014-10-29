#version 130

in vec4 vPosition;
out vec4 color;

void main()
{
    color = vec4((1.0 + vPosition.xyz)/2.0, 1.0);
    gl_Position = vPosition;
}