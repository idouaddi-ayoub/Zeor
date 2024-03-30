#shader vertex
#version 330 core

layout(location = 0) in vec4 pos;

void main()
{
   gl_Position = pos;
};
    
#shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

void main()
{
   FragColor = vec4(0.5f, 0.5f, 0.0f, 1.0f);
};