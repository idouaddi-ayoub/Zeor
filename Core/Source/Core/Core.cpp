#include "Core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace Core 
{

	void testFunc()
	{
		int x{ getUserValue() }, y{ getUserValue() };
		std::cout << Calc::multiple(x, y);
		std::cout << Calc::sum(x, y);
	}

	int getUserValue()
	{
		std::cout << "\nEnter int\n";

		int input{};
		std::cin >> input;

		return input;
	}

}

namespace Calc {

	int multiple(int x, int y)
	{
		std::cout << "\nMutliple: ";
		return x * y;
	}

	int sum(int x, int y)
	{
		std::cout << "\nSummation: ";
		return x + y;
	}

}

namespace openGL {

    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void processInput(GLFWwindow* window);

    unsigned int SCR_WIDTH{ 800 };
    unsigned int SCR_HEIGHT{ 600 };

    int opengl(void)
    {
        /* Initialize the library */
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        /* Create a windowed mode window and its OpenGL context */
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

        /*glad*/
      if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "failed to initialize gald" << std::endl;

            return -1;
        }

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /*input*/
            processInput(window);

            /* Render here */
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        glfwTerminate();

        return 0;
    }

    void processInput(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }
}