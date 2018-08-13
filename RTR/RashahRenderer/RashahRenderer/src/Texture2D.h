#pragma once

#include "TextureParams.h"

#include <stb_image/stb_image.h>

#include "GL/glew.h"

#include <string>
#include <iostream>

class Texture2D
{
public:
	Texture2D(TextureParams &params, const std::string &path);
	~Texture2D();

	void bind(GLuint unit);
	void unbind();
private:
	void loadFromFile(const std::string &path);
	void upload();
	void configurate();
	void generateMipMap();

	TextureParams _params;

	unsigned char* _data;
	GLint _format;
	GLenum _dataType;
	GLsizei _width;
	GLsizei _height;

	GLuint _handle;
};