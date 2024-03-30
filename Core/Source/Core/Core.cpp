#include "Core.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

std::string res = "C:/Users/PrOeZ/Project/WindowsApp/Core/res/";

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    
    enum class ShaderType
    {
        NONE     = -1,
        VERTEX   =  0,
        FRAGMENT =  1,
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << std::endl;
        }
    }

    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        { 
            int lenght;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
            char* message = (char*)_malloca(lenght * sizeof(char));
            glGetShaderInfoLog(id, lenght, &lenght, message);
            std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "Shader!" << 
                std::endl;
            std::cout << message << std::endl;
            glDeleteShader(id);
            return 0;
        }

        return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

namespace openGL {

    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void processInput(GLFWwindow* window);

    unsigned int SCR_WIDTH{ 800 };
    unsigned int SCR_HEIGHT{ 600 };

    int opengl(void)
    {
        /* Initialize the library */
        if (!glfwInit())
        {
            return -1;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        /* Create a windowed mode window and its OpenGL's context */
        GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lbzboz", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Error creating the window" << std::endl;
            glfwTerminate();
            return -1;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "failed to initialize GLAD" << std::endl;

            return -1;
        }

        // build and compile our shader program
        // ------------------------------------
        
        //set up vertex data
        float vertices[] = 
        {
            // Drawing triangle anticlockwise
            // X     Y     Z
            -0.5f, -0.5f, // 0
             0.5f, -0.5f, // 1
             0.5f,  0.5f, // 2
            -0.5f,  0.5f, // 3
        };

        unsigned int indices[] = 
        {
         //v0, v1, v2
            0, 1, 2,
            2, 3, 0
        };

        unsigned int VBO{}, VAO{}, EBO{};

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        /* 1. Biding array */
        glBindVertexArray(VAO);

        /* 2. Copy the vertices array in a buffer */
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * 6, vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * 6 , indices, GL_STATIC_DRAW);

        /* 3. Setting vertex attributes pointers */
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
        glEnableVertexAttribArray(0);
        
        // vertex shader
        ShaderProgramSource source = ParseShader(res + "shaders/Basic.shader");
        std::cout << "VERTEX" << std::endl;
        std::cout << source.VertexSource << std::endl;
        std::cout << "FRAGMENT" << std::endl;
        std::cout << source.FragmentSource<< std::endl;

        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        glUseProgram(shader);
        /* 4. Drawing the object */
        glBindBuffer(GL_ARRAY_BUFFER, 0);//unbind
        glBindVertexArray(0);//unbind
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Input */
            processInput(window);

            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            /* Drawing triangle */
            glBindVertexArray(VAO);

            //glDrawArrays(GL_TRIANGLES, 0, 6); // without index buffer
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); // with index buffer

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        /* De-allocate all resources once they've outlived their purpose */
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);

        /* Destroy Window & terminate allocated GLFW resources */ 
        glfwDestroyWindow(window);

        glDeleteProgram(shader);
        glfwTerminate();
        return 0;
    }

    /* process all input */
    void processInput(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    /* glfw: whenever the window size changed (by OS or user resize) this callback function executes */
    /* --------------------------------------------------------------------------------------------- */
    void framebuffer_size_callback(GLFWwindow*, int width, int height)
    {
        /* make sure the viewport matches the new window dimensions; note that width and */
        /* height will be significantly larger than specified on retina displays. */
        glViewport(0, 0, width, height);
    }
}