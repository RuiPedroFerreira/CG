#pragma once


#include "Cube.h"
#include <cstdlib>
#include <ctime>


struct ActiveFigure
{
	glm::vec3 pos;
	Cube** form;

	const float scale_factor = 0.3f;

	ActiveFigure()
	{
		form = new Cube * [4 * 4];
		std::memset(form, 0, sizeof(Cube*) * 4 * 4);
	}

	//eliminar peças que não estejam ativas
	~ActiveFigure()
	{
		for (int i = 0; i < 4 * 4; i++)
			delete form[i];
		delete form;
	}

	//construir peças de jogo
	void GenerateNew(const glm::vec3& frame_pos, float cube_size, int frame_width, int frame_height)
	{
		
		//Define os primeiros num bytes do bloco de memória apontado por ptr para o valor especificado (interpretado como um caractere sem sinal ).
		
		std::memset(form, 0, sizeof(Cube*) * 4 * 4);

		
		//vetor que guarda as cores do vetor
		const static glm::vec3 colors[3] = { {1.0f, 0.3f, 0.6f}, {1.0f, 0.8f, 0.7f}, {0.5f, 1.0f, 0.5f} };
		srand((unsigned int)(time(0)));
		
		int x = 1 + rand() % (frame_width - 2);
		int y =-5;

		pos = frame_pos;
		pos.x += x;
		pos.y += -y;

		int f     = 0 + rand() % 3;
		int color = 0 + rand() % 3;

		if (f == 0) {
			int index = 0;
			form[index] = new Cube();
			form[index]->SetPosition(glm::vec3(
				frame_pos.x + x * cube_size,
				pos.y - 0 * cube_size,
				frame_pos.z
			));
			form[index]->SetColor(colors[color]);
			form[index]->Scale(scale_factor);

			index = 1 * 4 + 0;
			form[index] = new Cube();
			form[index]->SetPosition(glm::vec3(
				frame_pos.x + x * cube_size,
				pos.y - 1 * cube_size,
				frame_pos.z
			));
			form[index]->SetColor(colors[color]);
			form[index]->Scale(scale_factor);

			index = 2 * 4 + 0;
			form[index] = new Cube();
			form[index]->SetPosition(glm::vec3(
				frame_pos.x + x * cube_size,
				pos.y - 2 * cube_size,
				frame_pos.z
			));
			form[index]->SetColor(colors[color]);
			form[index]->Scale(scale_factor);

			index = 3 * 4 + 0;
			form[index] = new Cube();
			form[index]->SetPosition(glm::vec3(
				frame_pos.x + x * cube_size,
				pos.y - 3 * cube_size,
				frame_pos.z
			));
			form[index]->SetColor(colors[color]);
			form[index]->Scale(scale_factor);
		}
		else if (f == 1) {
			int index = 0;
			form[index] = new Cube();
			form[index]->SetPosition(glm::vec3(
				pos.x + 0 * cube_size,
				pos.y - 0 * cube_size,
				frame_pos.z
			));
			form[index]->SetColor(colors[color]);
			form[index]->Scale(scale_factor);

			index = 1;
			form[index] = new Cube();
			form[index]->SetPosition(glm::vec3(
				pos.x + 1 * cube_size,
				pos.y - 0 * cube_size,
				frame_pos.z
			));
			form[index]->SetColor(colors[color]);
			form[index]->Scale(scale_factor);

			index = 4 * 1;
			form[index] = new Cube();
			form[index]->SetPosition(glm::vec3(
				pos.x + 0 * cube_size,
				pos.y - 1 * cube_size,
				frame_pos.z
			));
			form[index]->SetColor(colors[color]);
			form[index]->Scale(scale_factor);

			index = 4 * 1 + 1;
			form[index] = new Cube();
			form[index]->SetPosition(glm::vec3(
				pos.x + 1 * cube_size,
				pos.y - 1 * cube_size,
				frame_pos.z
			));
			form[index]->SetColor(colors[color]);
			form[index]->Scale(scale_factor);
		}
		else if (f == 2) {
			int index = 0;
			form[index] = new Cube();
			form[index]->SetPosition(glm::vec3(
				pos.x + 0 * cube_size,
				pos.y - 0 * cube_size,
				frame_pos.z
			));
			form[index]->SetColor(colors[color]);
			form[index]->Scale(scale_factor);

			index = 1 * 4;
			form[index] = new Cube();
			form[index]->SetPosition(glm::vec3(
				pos.x + 0 * cube_size,
				pos.y - 1 * cube_size,
				frame_pos.z
			));
			form[index]->SetColor(colors[color]);
			form[index]->Scale(scale_factor);

			index = 2 * 4;
			form[index] = new Cube();
			form[index]->SetPosition(glm::vec3(
				pos.x + 0 * cube_size,
				pos.y - 2 * cube_size,
				frame_pos.z
			));
			form[index]->SetColor(colors[color]);
			form[index]->Scale(scale_factor);

			index = 3 * 4;
			form[index] = new Cube();
			form[index]->SetPosition(glm::vec3(
				pos.x + 0 * cube_size,
				pos.y - 3 * cube_size,
				frame_pos.z
			));
			form[index]->SetColor(colors[color]);
			form[index]->Scale(scale_factor);

			index = 3 * 4 + 1;
			form[index] = new Cube();
			form[index]->SetPosition(glm::vec3(
				pos.x + 1 * cube_size,
				pos.y - 3 * cube_size,
				frame_pos.z
			));
			form[index]->SetColor(colors[color]);
			form[index]->Scale(scale_factor);
		}
	}

	// função quue permite que o controlo da peça para o lado esquerdo
	bool MoveRight(const glm::vec3& frame_pos, float cube_size, Cube** falled_cubes, int frame_width, int frame_height)
	{
		Move(glm::vec3(cube_size, 0.0f, 0.0f));
		if (CheckCollision(frame_pos, cube_size, falled_cubes, frame_width, frame_height)) {
			Move(glm::vec3(-cube_size, 0.0f, 0.0f));
			return false;
		}
		return true;
	}

	// função quue permite que o controlo da peça para o lado esquerdo
	bool MoveLeft(const glm::vec3& frame_pos, float cube_size, Cube** falled_cubes, int frame_width, int frame_height)
	{
		Move(glm::vec3(-cube_size, 0.0f, 0.0f));
		if (CheckCollision(frame_pos, cube_size, falled_cubes, frame_width, frame_height)) {
			Move(glm::vec3(cube_size, 0.0f, 0.0f));
			return false;
		}
		return true;
	}

	// função que permite que a peça cai logo na posição
	bool MoveDown(const glm::vec3& frame_pos, float cube_size, Cube** falled_cubes, int frame_width, int frame_height)
	{
		Move(glm::vec3(0.0f, -cube_size, 0.0f));
		if (CheckCollision(frame_pos, cube_size, falled_cubes, frame_width, frame_height)) {
			Move(glm::vec3(0.0f, cube_size, 0.0f));
			return false;
		}
		return true;
	}

	//mudar a posição das peças
	void RotateClockWise(const glm::vec3& frame_pos, float cube_size, Cube** falled_cubes,
		int frame_width, int frame_height)
	{
		Cube* tmp_matrix[4][4];
		glm::vec3 old_positions[4 * 4];

		for (int y = 0; y < 4; y++)
			for (int x = 0; x < 4; x++)
				tmp_matrix[y][x] = form[y * 4 + x];

		int n = 4;
		for (int i = 0; i < n / 2; i++)
			for (int j = 0; j < (n + 1) / 2; j++)
				CyclicRoll(&tmp_matrix[i][j], &tmp_matrix[n - 1 - j][i],
					&tmp_matrix[n - 1 - i][n - 1 - j], &tmp_matrix[j][n - 1 - i]);

		for (int y = 0; y < 4; y++)
			for (int x = 0; x < 4; x++)
				if (tmp_matrix[y][x] != nullptr) {
					old_positions[y * 4 + x] = tmp_matrix[y][x]->GetPosition();
					glm::vec3 new_pos = glm::vec3(cube_size * x, cube_size * (y), 0.0f) + pos;
					tmp_matrix[y][x]->SetPosition(new_pos);
				}

		if (CheckCollision(frame_pos, cube_size, falled_cubes, frame_width, frame_height)) {
			for (int y = 0; y < 4; y++)
				for (int x = 0; x < 4; x++)
					if (tmp_matrix[y][x] != nullptr) {
						tmp_matrix[y][x]->SetPosition(old_positions[y * 4 + x]);
					}
		}
		else {
			for (int y = 0; y < 4; y++)
				for (int x = 0; x < 4; x++)
					form[y * 4 + x] = tmp_matrix[y][x];
		}
	}


private:

	//função que vai permitir que os blocos se movam
	void Move(const glm::vec3& mov)
	{
		pos = pos + mov;
		for (int i = 0; i < 4 * 4; i++)
			if (form[i] != 0) {
				glm::vec3 pos = form[i]->GetPosition() + mov;
				form[i]->SetPosition(pos);
			}
	}

	void CyclicRoll(Cube** a, Cube** b, Cube** c, Cube** d)
	{
		Cube* temp = *a;
		*a = *b;
		*b = *c;
		*c = *d;
		*d = temp;
	}

	//verificar colisões dos blocos
	bool CheckCollision(const glm::vec3& frame_pos, float cube_size,
		Cube** falled_cubes, int frame_width, int frame_height)
	{
		for (int i = 0; i < 4 * 4; i++)
			if (form[i] != nullptr) {
				glm::vec3 block_pos = form[i]->GetPosition();

				int xx = (int)((block_pos.x - frame_pos.x) / cube_size);
				if (xx < 0 || xx >= frame_width)
					return true;

				int yy = (int)((frame_pos.y - block_pos.y) / cube_size - 1);
				if (yy >= frame_height)
					return true;

				if (yy >= 0 && falled_cubes[frame_width * yy + xx] != 0)
					return true;
			}
		return false;
	}
};