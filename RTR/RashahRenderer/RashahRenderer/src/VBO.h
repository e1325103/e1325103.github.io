#pragma once
#include <GL\glew.h>

struct BUFFER_TYPE_DATA {
	GLenum type;
	GLenum mode;
	GLuint index;
	GLenum dataType;
	GLint typeSize;
	GLint components;
};

static BUFFER_TYPE_DATA _BUFFER_DATA[] = {
	{ GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, 0, GL_UNSIGNED_INT, sizeof(unsigned int), 1},
	{ GL_ARRAY_BUFFER, GL_STATIC_DRAW, 0, GL_FLOAT, sizeof(float), 3 },
	{ GL_ARRAY_BUFFER, GL_STATIC_DRAW, 1, GL_FLOAT, sizeof(float), 3 },
	{ GL_ARRAY_BUFFER, GL_STATIC_DRAW, 2, GL_FLOAT, sizeof(float), 3 },
	{ GL_ARRAY_BUFFER, GL_STATIC_DRAW, 3, GL_FLOAT, sizeof(float), 2 }
};

class VBO {
public:
	enum class BUFFER_TYPE {
		INDEX = 0, POSITIONS = 1, NORMALS = 2, COLORS = 3, UVS = 4
	};


	VBO(BUFFER_TYPE type, int vertices, void* data);
	~VBO();

	void bind() const;
	void bindToVAO() const;
	unsigned int getLocation() const;
private:
	GLuint _handle;
	BUFFER_TYPE_DATA* _type;
	int _vertices;
};