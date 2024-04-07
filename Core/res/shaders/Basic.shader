#shader vertex
#version 330 core

layout(location = 0) in vec4 vecPos;

void main()
{
   gl_Position = vecPos;
};
    
#shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

uniform vec4 u_Color;

void main()
{
   FragColor = u_Color;
};