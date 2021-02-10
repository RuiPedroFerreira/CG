#include "Camera.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <GLM/gtc/quaternion_simd.inl>


Camera::Camera()
{
	projection = glm::perspective(glm::radians(35.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	camera_view = glm::lookAt(
		glm::vec3(0.0f,-4.0f, 50.0f), // Camera is at (4,3,-3), in World Space
		glm::vec3(0.0f, -4.0f, 0.0f), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
}


Camera::~Camera()
{

}



glm::mat4 Camera::GetProjectionMatrix() const
{
	return projection;
}


glm::mat4 Camera::GetViewMatrix() const
{
	return camera_view;
}


void Camera::SetPerspectiveWidthAndHeight(int width, int height)
{
	projection = glm::perspective(glm::radians(35.0f), (float)width / (float)height, 0.1f, 100.0f);
}


void Camera::Rotate(const glm::vec3& axis, float degrees)
{
	
	glm::quat quatrion = glm::quat(axis.x, axis.y, axis.z, degrees);
	quatrion = glm::angleAxis(degrees, axis);
	camera_view = camera_view * glm::mat4(quatrion);
}


void Camera::Move(const glm::vec3& v)
{
	camera_view = glm::translate(camera_view, v);
}