#include "CubeRenderer.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Camera.h"

// compilar e executar os shaders
CubeRenderer::CubeRenderer()
{
	const char* vertex_file_path = "..\\..\\Shaders\\CubeVertexShader.cpp";
	const char* fragment_file_path = "..\\..\\Shaders\\CubeFragmentShader.cpp";

	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	if (ProgramID) {
		printf("Cube program compiled!\n");
	}

	program = ProgramID;

	proj_view_location    = glGetUniformLocation(program, "view_projection");
	model_matrix_location = glGetUniformLocation(program, "model_matrix");

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}


CubeRenderer::~CubeRenderer()
{
	glDeleteProgram(program);
}

#include <GLM/gtc/matrix_transform.hpp>


void CubeRenderer::Render(const Camera& camera, Cube* cubes, GLuint cubes_count)
{
	glUseProgram(GetProgram());

	GLuint vertices_count = cubes[0].GetVerticesCount();

	glm::mat4 projection = camera.GetProjectionMatrix();
	glm::mat4 camera_view = camera.GetViewMatrix();


	glm::mat4 camera_view_matrix = projection * camera_view;
	glUniformMatrix4fv(proj_view_location, 1, GL_FALSE, &camera_view_matrix[0][0]);

	for (GLuint i = 0; i < cubes_count; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, cubes[i].GetVbo());

		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)(sizeof(glm::vec3)));

		glUniformMatrix4fv(model_matrix_location, 1, GL_FALSE, &(cubes[i].GetModelMatrix())[0][0]);
		glDrawArrays(GL_LINES, 0, vertices_count);
	}
}


GLuint CubeRenderer::GetProgram() const
{
	return program;
}
