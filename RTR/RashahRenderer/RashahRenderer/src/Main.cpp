#include <iostream>
#include <vector>

#include "Window.h"
#include "Camera.h"

#include "VBO.h"
#include "Mesh.h"
#include "TriangleMesh.h"
#include "SceneObject.h"
#include "Scene.h"
#include "Shader.h"

void handleKeyEvent(KEY k, KEY_ACTION a);

Window window = Window();
Camera* camera;

int main()
{

	bool res = window.init();
	if (!res) {
		std::cout << "ERROR: Init Window :(" << std::endl;
		return EXIT_FAILURE;
	}
	window.setKeyChangedCallback(handleKeyEvent);
	res = window.open(800, 800, false, "The Lost Treasure of Alman-Rashah");

	camera = new Camera(glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.01f, 1000.0f));
	camera->lookAt(glm::vec3(0, 1, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	// TEST SCENE

	std::vector<GLuint> ind = std::vector<GLuint>();
	ind.push_back(0);
	ind.push_back(1);
	ind.push_back(2);
	ind.push_back(3);
	ind.push_back(4);
	ind.push_back(5);
	VBO* vbo_ind = new VBO(VBO::BUFFER_TYPE::INDEX, (int)ind.size(), &ind[0]);

	std::vector<glm::vec3> pos = std::vector<glm::vec3>();

	pos.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
	pos.push_back(glm::vec3(1.0f, -1.0f, 0.0f));
	pos.push_back(glm::vec3(-1.0f, 1.0f, 0.0f));
	pos.push_back(glm::vec3(-1.0f, 1.0f, 0.0f));
	pos.push_back(glm::vec3(1.0f, -1.0f, 0.0f));
	pos.push_back(glm::vec3(1.0f, 1.0f, 0.0f));

	VBO* vbo_pos = new VBO(VBO::BUFFER_TYPE::POSITIONS, (int)pos.size(), &pos[0]);
	std::vector<VBO*> vbos;
	vbos.push_back(vbo_pos);
	vbos.push_back(vbo_ind);
	Mesh* mesh = new TriangleMesh(vbos, 6);
	SceneObject* obj = new SceneObject(mesh, glm::mat4(1.0f));

	Scene* scene = new Scene();
	scene->addSceneObject(obj);

	SHADER_TYPE types[] = { SHADER_TYPE::VERTEX, SHADER_TYPE::FRAGMENT };
	Shader* shader = new Shader("..\\shader\\Texture", types);
	shader->setUniform("targetColor", glm::vec3(0.5, 1.0, 1.0));
	// ***

	while (!window.shouldClose())
	{
		glClearColor(0, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->bind();
		scene->render(camera);

		//scene->render(camera);

		window.refresh();
	}

	if (!res) {
		std::cout << "ERROR: Open Window :(" << std::endl;
		return EXIT_FAILURE;
	}

	return 0;
}

void handleKeyEvent(KEY k, KEY_ACTION a) {
	switch (k)
	{
	case KEY::ESC:
		std::cout << "ESC" << std::endl;
		window.close();
		break;
	default:
		break;
	}
}