#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
	Camera(glm::mat4 proj);
	~Camera();

	void lookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up);

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
private:
	glm::mat4 _view;
	glm::mat4 _proj;

	glm::vec3 _position;
};