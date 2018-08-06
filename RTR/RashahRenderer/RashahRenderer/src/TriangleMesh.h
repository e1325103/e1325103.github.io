#pragma once

#include "Mesh.h"

class TriangleMesh : public Mesh {
public:
	TriangleMesh(std::vector<VBO*> buffers, GLsizei size);
	virtual ~TriangleMesh();
protected:
	virtual void render();
};