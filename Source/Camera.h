#pragma once


#include <GLM/glm.hpp>


class Camera
{
public:

	Camera();
	~Camera();

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

	void SetPerspectiveWidthAndHeight(int width, int height);

	void Rotate(const glm::vec3& axis, float degrees);
	void Move(const glm::vec3& v);

private:

	glm::mat4 projection;
	glm::mat4 camera_view;
};