#pragma once
#include <vector>

#include "VBO.h"

class VAO
{
public:
	VAO();
	~VAO();

	void bind();
	void unbind();

	void setBuffers(std::vector<VBO*> buffers);
private:
	GLuint _handle;
};