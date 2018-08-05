#include "VBO.h"

VBO::VBO(BUFFER_TYPE type, int vertices, void* data)
{
	_vertices = vertices;
	_type = &_BUFFER_DATA[(int)type];

	glGenBuffers(1, &_handle);
	glBindBuffer(_type->type, _handle);
	glBufferData(_type->type, _type->typeSize * _type->components * vertices, data, _type->mode);
}

VBO::~VBO()
{
	glDeleteBuffers(1, &_handle);
}

void VBO::bind() const
{
	glBindBuffer(_type->type, _handle);
}

void VBO::bindToVAO() const
{
	glBindBuffer(_type->type, _handle);

	if (_type->type != GL_ELEMENT_ARRAY_BUFFER) { // NOT for Index Buffer Objects
		glEnableVertexAttribArray(_type->index);
		glVertexAttribPointer(_type->index, _type->components, _type->dataType, GL_FALSE, 0, 0);
	}
}

unsigned int VBO::getLocation() const
{
	return _type->index;
}