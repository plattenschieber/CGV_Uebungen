#version 130

uniform float time;
in vec4 vPosition;
void main()
{

    vec4 temp = vPosition;
    temp.x *= (1+sin(time));
    gl_Position = temp;
}