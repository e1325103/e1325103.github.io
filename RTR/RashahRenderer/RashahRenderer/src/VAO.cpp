#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &_handle);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &_handle);
}

void VAO::bind()
{
	glBindVertexArray(_handle);
}

void VAO::unbind()
{
	glBindVertexArray(0);
}

void VAO::setBuffers(std::vector<VBO*> buffers)
{
	bind();
	for (const VBO* vbo : buffers) {
		vbo->bindToVAO();
	}
	unbind();
}