#shader vertex
#version 330 core

layout(location = 0) in vec4 vexPos;

void main()
{
   gl_Position = vexPos;
};
    
#shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

void main()
{
   FragColor = vec4(1.0, 0.0, 0.0, 1.0);
};