#include "SceneObject.h"

SceneObject::SceneObject(Mesh* mesh, glm::mat4 model) : _mesh(mesh), _model(model)
{

}

SceneObject::~SceneObject()
{
	delete _mesh;
}

glm::mat4 SceneObject::getModelMatrix() {
	return _model;
}

void SceneObject::render(const glm::mat4& V, const glm::mat4& P)
{
	_mesh->render(_model, V, P);
}