#pragma once

#include <GL/glew.h>
#include "Cube.h"

class Camera;

class CubeRenderer
{
public:

	CubeRenderer();
	~CubeRenderer();

	void Render(const Camera& camera, Cube* cubes, GLuint cubes_count);

	GLuint GetProgram() const;

private:

	GLuint program;
	GLint proj_view_location, model_matrix_location;
};