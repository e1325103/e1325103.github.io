#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Mesh.h"

class SceneObject {
public:
	SceneObject(Mesh* mesh, glm::mat4 model);
	~SceneObject();

	glm::mat4 getModelMatrix();
	void render(const glm::mat4& V, const glm::mat4& P);
private:
	glm::mat4 _model;

	Mesh* _mesh;
};