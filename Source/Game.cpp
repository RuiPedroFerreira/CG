#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include "Game.h"
#include "Window.h"
#include "CubeRenderer.h"
#include "Cube.h"
#include "Camera.h"
#include "ActiveFigure.h"

//sound
#include <irrKlang/irrKlang.h>
//sound
irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();


Game::Game()
{
	window = 0;
}


Game::~Game()
{
	Free();
}


void Game::Init()
{
	//GLuint textShader = initFont();

	/*if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD\n";
		return;
	}*/

	glfwInit();

	const int window_width = 1280;
	const int window_height = 720;

	window = new Window();
	window->SetSize(WindowSize(window_width, window_height));
	window->SetKeyboardCallback(this, KeyboardCallback);
	window->SetResizeCallback(this, WindowResizeCallback);
	window->SetMousePositionCallback(this, CursorPositionCallback);

	glewInit();

	cube_renderer = new CubeRenderer();
	SetUpFieldFrame();

	camera = new Camera();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	camera->SetPerspectiveWidthAndHeight(window_width, window_height);
	glViewport(0, 0, window_width, window_height);

	active_figure = new ActiveFigure();
	active_figure->GenerateNew(field_frame.cubes_pos, cube_size, field_frame.width, field_frame.height);

	falled_cubes.buffer_size = field_frame.width * field_frame.height;
	falled_cubes.buffer = new Cube * [falled_cubes.buffer_size];
	std::memset(falled_cubes.buffer, 0, sizeof(Cube*) * falled_cubes.buffer_size);

	window->GetCursorPos(cursor_pos_x, cursor_pos_y);

	pause = false;
	
	// Text Rendering
	//RenderText(textShader, "Projeto CG",200, 200, 1.0f, glm::vec3(0.145, 0.564, 0.658));
	
	//play sound
	SoundEngine->play2D("../sounds/main.wav", true);
}



//libertar os dados "armazenados" no final do jogo
void Game::Free()
{
	if (falled_cubes.buffer != 0)
		for (unsigned int i = 0; i < falled_cubes.buffer_size; i++)
			delete falled_cubes.buffer[i];

	delete[] falled_cubes.buffer;
	falled_cubes.buffer = 0;

	delete active_figure;
	active_figure = 0;

	delete camera;
	camera = 0;

	delete[] field_frame.cubes;
	field_frame.cubes = 0;

	delete cube_renderer;
	cube_renderer = 0;

	delete window;
	window = 0;

	glfwTerminate();
}


void Game::Run()
{
	double cur_tick;
	double prev_tick = glfwGetTime();
	double delta = 0;

	while (window->IsOpened() && !GameOver())
	{
		glfwPollEvents();

		if (!pause) {
			cur_tick = glfwGetTime();
			delta += cur_tick - prev_tick;

			if (delta > timings.delay) {
				if (active_figure->MoveDown(field_frame.pos, cube_size, falled_cubes.buffer,
					field_frame.width, field_frame.height) == false) {
					SaveFalledFigure();
					active_figure->GenerateNew(field_frame.cubes_pos, cube_size,
						field_frame.width * cube_size, field_frame.height * cube_size);
				}
				delta = 0;
			}

			prev_tick = cur_tick;
		}
		Render();
		window->UpdateFrameBuffer();
	}
}


void Game::ErrorCallback(int error_code, const char* error_description)
{
	fprintf(stderr, "Error code %d, error description:\n%s\n\n", error_code, error_description);
}


void Game::Render() const
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cube_renderer->Render(*camera, field_frame.cubes, field_frame.cubes_count);

	for (int i = 0; i < 4 * 4; i++) {
		if (active_figure->form[i] != 0)
			cube_renderer->Render(*camera, active_figure->form[i], 1);
	}

	for (unsigned int i = 0; i < falled_cubes.buffer_size; i++)
		if (falled_cubes.buffer[i] != 0)
			cube_renderer->Render(*camera, falled_cubes.buffer[i], 1);
}


void Game::KeyboardCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	Game* game = (Game*)(glfwGetWindowUserPointer(window));
	static unsigned int key_press_count = 0;

	if (action == GLFW_PRESS) {
		game->KeyPressHandler(key, scancode, action, mods);
		key_press_count++;
	}
	else if (action == GLFW_REPEAT) {
		if (key_press_count > 1)
			game->KeyPressHandler(key, scancode, action, mods);
		key_press_count++;
	}
	else
		key_press_count = 0;
}


void Game::KeyPressHandler(int key, int scancode, int action, int mods)
{
	switch (key)
	{
	case GLFW_KEY_UP:
		active_figure->RotateClockWise(field_frame.pos, cube_size, falled_cubes.buffer,
			field_frame.width, field_frame.height);
		break;
	case GLFW_KEY_ESCAPE:
		window->Close();
		break;
	case GLFW_KEY_W:
		camera->Move(glm::vec3(0.0f, 0.0f, camera_moving_speed));
		return;
	case GLFW_KEY_D:
		camera->Move(glm::vec3(-camera_moving_speed, 0.0f, 0.0f));
		return;
	case GLFW_KEY_S:
		camera->Move(glm::vec3(0.0f, 0.0f, -camera_moving_speed));
		return;
	case GLFW_KEY_A:
		camera->Move(glm::vec3(camera_moving_speed, 0.0f, 0.0f));
		return;
	}

	if (!pause) {
		switch (key)
		{
		case GLFW_KEY_P:
			pause = !pause;
			break;
		case GLFW_KEY_RIGHT:
			active_figure->MoveRight(field_frame.pos, cube_size, falled_cubes.buffer,
				field_frame.width, field_frame.height);
			break;
		case GLFW_KEY_LEFT:
			active_figure->MoveLeft(field_frame.pos, cube_size, falled_cubes.buffer,
				field_frame.width, field_frame.height);
			break;
		case GLFW_KEY_DOWN:
			while (active_figure->MoveDown(field_frame.pos, cube_size, falled_cubes.buffer,
				field_frame.width, field_frame.height));
			break;
		default:
			break;
		}
	}
	else {
		switch (key)
		{
		case GLFW_KEY_P:
			pause = !pause;
			break;
		default:
			break;
		}
	}
}


void Game::WindowResizeCallback(GLFWwindow * window, int width, int height)
{
	Game* game = (Game*)(glfwGetWindowUserPointer(window));
	game->camera->SetPerspectiveWidthAndHeight(width, height);
	glViewport(0, 0, width, height);
}

//função que permite mover a janela de jogo a partir com o rato
void Game::CursorPositionCallback(GLFWwindow * window, double x, double y)
{
	Game* game = (Game*)(glfwGetWindowUserPointer(window));
	if (abs(game->cursor_pos_x - x) > abs(game->cursor_pos_y - y))
		game->camera->Rotate(glm::vec3(0.0f, 1.0, 0.0f),
			game->camera_rotation_speed * ((game->cursor_pos_x - x) / abs(game->cursor_pos_x - x)));
	else
		game->camera->Rotate(glm::vec3(1.0f, 0.0f, 0.0f), game->camera_rotation_speed *
		((game->cursor_pos_y - y) / abs(game->cursor_pos_y - y)));
	game->cursor_pos_x = x;
	game->cursor_pos_y = y;
}


//função que permite a atualização das posições aquando uma peça cai
void Game::SetUpFieldFrame()
{
	field_frame.cubes = new Cube[field_frame.cubes_count];

	unsigned int i;
	for (i = 0; i < field_frame.width * 2; i += 2)
	{
		field_frame.cubes[i].SetPosition(glm::vec3(
			field_frame.pos.x + cube_size * ((i / 2)),
			field_frame.pos.y,
			field_frame.pos.z
		));
		field_frame.cubes[i + 1].SetPosition(glm::vec3(
			field_frame.pos.x + cube_size * (i / 2),
			field_frame.pos.y - cube_size * field_frame.height - cube_size,
			field_frame.pos.z
		));
	}

	for (i = field_frame.width * 2; i < field_frame.cubes_count - 2; i += 2)
	{
		field_frame.cubes[i].SetPosition(glm::vec3(
			field_frame.pos.x - cube_size,
			field_frame.pos.y - cube_size * ((i - field_frame.width * 2) / 2),
			field_frame.pos.z
		));
		field_frame.cubes[i + 1].SetPosition(glm::vec3(
			field_frame.pos.x + cube_size * field_frame.width,
			field_frame.pos.y - cube_size * ((i - field_frame.width * 2) / 2),
			field_frame.pos.z
		));
	}

	field_frame.cubes[field_frame.cubes_count - 1].SetPosition(glm::vec3(
		field_frame.pos.x - cube_size,
		field_frame.pos.y - cube_size * (field_frame.height + 1),
		field_frame.pos.z
	));

	field_frame.cubes[field_frame.cubes_count - 2].SetPosition(glm::vec3(
		field_frame.pos.x + cube_size * field_frame.width,
		field_frame.pos.y - cube_size * (field_frame.height + 1),
		field_frame.pos.z
	));

	for (int i = 0; i < field_frame.cubes_count; i++)
		field_frame.cubes[i].Scale(field_frame.scale_factor);
}

// "guardar" uma figura no momento da colisão
void Game::SaveFalledFigure()
{
	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++)
			if (active_figure->form[y * 4 + x] != 0) {
				glm::vec3 pos = active_figure->form[y * 4 + x]->GetPosition();
				int xx = (int)((pos.x - field_frame.pos.x) / cube_size);
				int yy = (int)((field_frame.pos.y - pos.y) / cube_size - 1);
				if (y >= 0) {
					falled_cubes.buffer[yy * field_frame.width + xx] = active_figure->form[y * 4 + x];
				}
				else
					delete active_figure->form[y * 4 + x];
			}
	PushDownFalledCubes();
}


// tirar uma linha completa
void Game::PushDownFalledCubes()
{
	for (int y = field_frame.height - 1; y > 0; y--)
	{
		bool row_is_full = true;
		for (unsigned int x = 0; row_is_full && x < field_frame.width; x++)
			if (falled_cubes.buffer[y * field_frame.width + x] == nullptr)
				row_is_full = false;
		if (row_is_full) {
			for (unsigned int x = 0; x < field_frame.width; x++)
			{
				delete falled_cubes.buffer[y * field_frame.width + x];
				falled_cubes.buffer[y * field_frame.width + x] = nullptr;
			}
			for (int i = y; i > 0; i--)
			{
				for (unsigned int j = 0; j < field_frame.width; j++)
				{
					Cube* cube = falled_cubes.buffer[(i - 1) * field_frame.width + j];
					if (cube != nullptr) {
						falled_cubes.buffer[i * field_frame.width + j] = cube;
						cube->SetPosition(glm::vec3(0.0f, -cube_size, 0.0f) + cube->GetPosition());
						falled_cubes.buffer[(i - 1) * field_frame.width + j] = nullptr;
					}
				}
			}
			y++;
		}
	}
}


bool Game::GameOver()
{
	for (int i = 0; i < field_frame.width; i++)
		if (falled_cubes.buffer[i] != nullptr)
			return true;
	return false;
}