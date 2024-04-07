#include "Core.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

std::string res = "C:/Users/PrOeZ/Project/WindowsApp/Core/res/";

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ")" << " " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

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
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

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

        // Sync the FPS with monitors refresh rate
        glfwSwapInterval(1);

        // build and compile our shader program
        // ------------------------------------
        // init glew
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            std::cout << "Couldn't initalize OpenGL" << std::endl;
            return false;
        }
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


        /* 1. Binding array */
        unsigned int VAO;
        GLCall(glGenVertexArrays(1, &VAO)); //VAO
        GLCall(glBindVertexArray(VAO));

        /* 2. Copy the vertices array in a buffer */
        unsigned int VBO{};
        GLCall(glGenBuffers(1, &VBO));     //VBO
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 2, vertices, GL_STATIC_DRAW));

        /* 3. Setting vertex attributes pointers */
        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0));

        /* 4. Copy the indices array in a buffer */
        unsigned int IBO{};
        GLCall(glGenBuffers(1, &IBO));      //IBO
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * 6 , indices, GL_STATIC_DRAW));

        // vertex shader
        ShaderProgramSource source = ParseShader(res + "shaders/Basic.shader");

        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        GLCall(glUseProgram(shader));
        
        GLCall(int location = glGetUniformLocation(shader, "u_Color"));

        ASSERT(location != -1);
        GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

        float r = 0.0f;
        float increment = 0.05f;

        /* 4. Drawing the object */
        GLCall(glBindVertexArray(0));//unbind
        GLCall(glUseProgram(0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));//unbind
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        std::cout << glGetString(GL_VERSION) << std::endl;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Input */
            processInput(window);

            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            /* Drawing triangle */
            GLCall(glUseProgram(shader));
            GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

            GLCall(glBindVertexArray(VAO));
            GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // with index buffer

            if (r > 1.0f) {
                increment = -0.05f;
            }
            else if (r < 0.0){
                increment = 0.05f;
            }

            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        /* De-allocate all resources once they've outlived their purpose */
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &IBO);

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