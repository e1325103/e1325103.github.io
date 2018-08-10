#include "Scene.h"

Scene::Scene()
{

}

void Scene::addSceneObject(SceneObject* object) {
	sceneObjects.push_back(object);
}

Scene::~Scene()
{
	for (unsigned int i = 0; i < sceneObjects.size(); i++) {
		delete sceneObjects[i];
	}
}

void Scene::render(Camera* camera)
{
	glm::mat4 V = camera->getViewMatrix();
	glm::mat4 P = camera->getProjectionMatrix();

	for (unsigned int i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->render(V, P);
	}
}