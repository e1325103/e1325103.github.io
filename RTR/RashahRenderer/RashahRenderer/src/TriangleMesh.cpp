#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(std::vector<VBO*> buffers, GLsizei size) : Mesh(buffers, size)
{

}

TriangleMesh::~TriangleMesh()
{
}

void TriangleMesh::render() const
{
	glDrawElements(GL_TRIANGLES, _size, GL_UNSIGNED_INT, 0);
}