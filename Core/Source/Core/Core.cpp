#include "Core.h"
#include <iostream>
#include <GLFW/glfw3.h>

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

    unsigned int SCR_WIDTH{ 640 };
    unsigned int SCR_HEIGHT{ 480 };

    int opengl(void)
    {

        /* Initialize the library */
        if (!glfwInit())
            return -1;

        /* Create a windowed mode window and its OpenGL context */
        GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello World", NULL, NULL);
        if (!window)
        {
            std::cout << "error creating the window" << std::endl;
            glfwTerminate();
            return -1;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
//add GLAD
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

}