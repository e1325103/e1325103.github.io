#include "Camera.h"

Camera::Camera(glm::mat4 proj) : _proj(proj)
{
	_view = glm::lookAt(glm::vec3(0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
}

Camera::~Camera()
{

}

void Camera::lookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up)
{
	_view = glm::lookAt(pos, target, up);
	_position = pos;
}

glm::mat4 Camera::getProjectionMatrix()
{
	return _proj;
}

glm::mat4 Camera::getViewMatrix()
{
	return _view;
}