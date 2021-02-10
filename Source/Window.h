#pragma once


struct GLFWwindow;


struct WindowSize
{
	int width, height;

	WindowSize();
	WindowSize(int window_width, int height);
};

class Window
{
public:

	Window();
	Window(WindowSize window_metrics);

	~Window();

	void SetSize(WindowSize);
	WindowSize GetSize() const;
	bool IsOpened() const;

	void Close();

	void GetCursorPos(double& x_pos, double& y_pos) const;

	void SetKeyboardCallback(
		void* game_ptr, void (*callback_function)(GLFWwindow*, int, int, int, int)
	);
	void SetResizeCallback(
		void* game_ptr, void(*callback_function)(GLFWwindow*, int, int)
	);
	void SetMousePositionCallback(
		void* game_ptr, void(*callback_function)(GLFWwindow*, double, double)
	);

	void UpdateFrameBuffer() const;

private:

	GLFWwindow* glfw_window;

	static void ResizeCallback(GLFWwindow* window, int widht, int height);

};

