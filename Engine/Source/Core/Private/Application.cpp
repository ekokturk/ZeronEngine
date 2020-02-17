// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "../Public/Application.h"
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace ZeronEngine
{
	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		std::cout << "Application Running..." << std::endl;
		GLFWwindow* window;

		/* Initialize the library */
		if (!glfwInit())
			return ;
		
		/* Create a windowed mode window and its OpenGL context */
		window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
		if (!window)
		{
			glfwTerminate();
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(window);
		
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			//return;
		}
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		glfwTerminate();
	}

	void Application::Destroy()
	{

	}
}

