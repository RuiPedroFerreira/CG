#include "Window.h"


#include <GL/glew.h>
#include <GLFW/glfw3.h>


Window::Window()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	glfw_window = glfwCreateWindow(512, 512, "PROJECTO TETRIS - CG", 0, 0);

	if (!glfw_window)
	{
		glfwTerminate();
	}

	glfwMakeContextCurrent(glfw_window);
	glfwSwapInterval(1);

	glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


Window::Window(WindowSize window_metrics)
{
	Window();
	SetSize(window_metrics);
}


Window::~Window()
{
	glfwDestroyWindow(glfw_window);
}


void Window::SetSize(WindowSize new_size)
{
	glfwSetWindowSize(glfw_window, new_size.width, new_size.height);
}


WindowSize Window::GetSize() const
{
	WindowSize window_size;
	glfwGetWindowSize(glfw_window, &window_size.width, &window_size.height);
	return window_size;
}


bool Window::IsOpened() const
{
	return !glfwWindowShouldClose(glfw_window);
}


void Window::UpdateFrameBuffer() const
{
	glfwSwapBuffers(glfw_window);
}


void Window::SetKeyboardCallback(
	void* game_ptr,
	void (*callback_function)(GLFWwindow*, int, int, int, int)
)
{
	glfwSetWindowUserPointer(glfw_window, game_ptr);
	glfwSetKeyCallback(glfw_window, callback_function);
}


void Window::SetResizeCallback(
	void* game_ptr,
	void(*callback_function)(GLFWwindow*, int, int)
)
{
	glfwSetWindowUserPointer(glfw_window, game_ptr);
	glfwSetFramebufferSizeCallback(glfw_window, callback_function);
}


void Window::SetMousePositionCallback(
	void* game_ptr,
	void(*callback_function)(GLFWwindow*, double, double)
)
{
	glfwSetWindowUserPointer(glfw_window, game_ptr);
	glfwSetCursorPosCallback(glfw_window, callback_function);
}


void Window::GetCursorPos(double& x_pos, double& y_pos) const
{
	glfwGetCursorPos(glfw_window, &x_pos, &y_pos);
}


void Window::Close()
{
	glfwSetWindowShouldClose(glfw_window, GLFW_TRUE);
}


WindowSize::WindowSize()
{
	width = 0;
	height = 0;
}


WindowSize::WindowSize(int window_width, int window_height)
{
	width = window_width;
	height = window_height;
}
