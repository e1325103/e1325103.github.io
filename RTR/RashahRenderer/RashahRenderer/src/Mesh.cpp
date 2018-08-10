#include "Mesh.h"

Mesh::Mesh(std::vector<VBO*> buffers, GLsizei size)
	: _buffers(buffers), _size(size)
{
	_vao = new VAO();
	_vao->setBuffers(buffers);
}

Mesh::~Mesh()
{
	for (int i = 0; i < 9; i++) {
		if (_buffers[i] != nullptr) delete _buffers[i];
	}
	delete _vao;
}

void Mesh::render(const glm::mat4 &M, const glm::mat4 &V, const glm::mat4 &P) {
	_vao->bind();

	render();
}