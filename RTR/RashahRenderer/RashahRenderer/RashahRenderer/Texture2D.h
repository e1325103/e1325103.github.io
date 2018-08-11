#pragma once

#include "stb_image/stb_image.h"
#include "GL/glew.h"

#include <string>
#include <iostream>

class Texture2D
{
public:
	Texture2D(const std::string &path);
	~Texture2D();

	void bind(GLuint unit);
	void unbind();
private:
	void loadFromFile(const std::string &path);
	void upload();

	unsigned char* _data;
	GLint _format;
	GLsizei _width;
	GLsizei _height;

	GLuint _handle;
};