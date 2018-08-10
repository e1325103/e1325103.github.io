#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "VAO.h"
#include "VBO.h"

class Mesh {
public:
	Mesh(std::vector<VBO*> buffers, GLsizei size);
	virtual ~Mesh();

	void render(const glm::mat4 &M, const glm::mat4 &V, const glm::mat4 &P);
protected:
	VAO * _vao;
	std::vector<VBO*> _buffers;
	size_t _size;

	virtual void render() const = 0;
};