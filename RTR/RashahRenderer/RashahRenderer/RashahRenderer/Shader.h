#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

enum SHADER_TYPE {
	VERTEX = 0,
	FRAGMENT = 1
};

class Shader {
public:
	Shader(std::string file, SHADER_TYPE types[]);
	virtual ~Shader();

	void bind();
	void unbind();

private:
	const static std::string SHADER_EXTENSION[];
	const static GLenum SHADER_GL_TYPE[];
	GLuint _handle;

	int loadShaderFromFile(std::string filename, SHADER_TYPE types);
};