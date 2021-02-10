#pragma once


#include "Objects.h"
#include <GL/glew.h>
#include <GLM/glm.hpp>


class Cube
{
public:

	Cube();
	~Cube();

	GLuint GetVbo() const;
	GLuint GetVerticesCount() const;

	void SetPosition(const glm::vec3& new_pos);
	void Rotate(float x_degrees, float y_degrees, float z_degrees);
	void SetColor(const glm::vec3& color);
	void Scale(float scale_factor);

	const glm::vec3 GetPosition() const;
	const glm::mat4 GetModelMatrix() const;


private:

	unsigned int vertices_count;
	Vertex3D* vertices;
	GLuint vbo;

	glm::mat4 model_matrix;
};