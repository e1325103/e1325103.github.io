#pragma once

#include "SceneObject.h"
#include "Camera.h"

#include <vector>

class Scene {
public:
	Scene();
	~Scene();

	void render(Camera* camera);
	void addSceneObject(SceneObject* object);
private:
	std::vector<SceneObject*> sceneObjects;
};