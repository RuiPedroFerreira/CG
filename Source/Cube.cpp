#include "Cube.h"

#include <GLM/gtc/matrix_transform.hpp>

//construção da janela do jogo
Cube::Cube()
{
#define PATTERN edge_cube_pattern
	vertices_count = sizeof(PATTERN) / sizeof(glm::vec3);
	vertices = new Vertex3D[vertices_count];

	for (int i = 0; i < vertices_count; i++)
	{
		vertices[i].pos = PATTERN[i];
		vertices[i].color = glm::vec3(0.0f, 1.0f, 1.0f);
	}

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D) * vertices_count, vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	model_matrix = glm::mat4(1.0f);
}


Cube::~Cube()
{
	glDeleteBuffers(1, &vbo);

	vertices_count = 0;
	delete[] vertices;
}


GLuint Cube::GetVbo() const
{
	return vbo;
}


GLuint Cube::GetVerticesCount() const
{
	return vertices_count;
}


void Cube::SetPosition(const glm::vec3& new_pos)
{
	model_matrix[3] = glm::vec4(new_pos, 1.0f);
}


void Cube::Scale(float scale_factor)
{
	model_matrix = glm::scale(model_matrix, glm::vec3(scale_factor, scale_factor, scale_factor));
}

const glm::vec3 Cube::GetPosition() const
{
	return glm::vec3(
		model_matrix[3][0],
		model_matrix[3][1],
		model_matrix[3][2]
	);
}


const glm::mat4 Cube::GetModelMatrix() const
{
	return model_matrix;
}


//atribuir as cores para os cubos
void Cube::SetColor(const glm::vec3& color)
{
	for (int i = 0; i < vertices_count; i++)
		vertices[i].color = color;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex3D) * vertices_count, vertices);
}

