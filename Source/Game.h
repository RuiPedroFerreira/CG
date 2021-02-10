#pragma once

#include <GLM\glm.hpp>

struct GLFWwindow;
class CubeRenderer;
class Camera;
class Cube;
struct ActiveFigure;


class Window;

class Game
{
public:

	Game();
	~Game();

	void Init();
	void Run();
	void Free();

private:

	Window* window;

	Camera* camera;
	CubeRenderer* cube_renderer;

	bool pause;

	const float camera_moving_speed   = 1.0f;
	const float camera_rotation_speed = 0.05f;

	const float cube_size = 1.0f;

	struct FieldFrame
	{
		const glm::vec3 pos = { -5.0f, 5.0f, 0.0f };
		const glm::vec3 cubes_pos = { -5.0f, 5.0f, -0.1f };

		const float scale_factor = 0.5f;

		const unsigned int width  = 10;
		const unsigned int height = 20;
		const unsigned int cubes_count = (width + height) * 2 + 4;

		Cube* cubes;

	} field_frame;

	ActiveFigure* active_figure;

	struct Timings
	{
		const double delay = 0.5;
	} timings;

	struct FalledCubes
	{
		unsigned int buffer_size;
		Cube** buffer;
	} falled_cubes;;

	double cursor_pos_x, cursor_pos_y;

	void Render() const;

	void SetUpFieldFrame();
	void GetNewActiveFigure();

	void SaveFalledFigure();
	void PushDownFalledCubes();
	void ErrorCallback(int error_code, const char* error_description);

	static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void KeyPressHandler(int key, int scancode, int action, int mods);

	static void WindowResizeCallback(GLFWwindow* window, int width, int height);
	static void CursorPositionCallback(GLFWwindow* window, double x, double y);

	bool GameOver();
	};