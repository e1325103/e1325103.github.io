#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>
#include <map>
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

	void setUniform(const std::string &name, int i);
	void setUniform(const std::string &name, unsigned int i);
	void setUniform(const std::string &name, float f);
	void setUniform(const std::string &name, const glm::mat3 &mat);
	void setUniform(const std::string &name, const glm::vec2 &vec);
	void setUniform(const std::string &name, const glm::mat4 &mat);
	void setUniform(const std::string &name, const glm::vec3 &vec);

private:
	const static std::string SHADER_EXTENSION[];
	const static GLenum SHADER_GL_TYPE[];
	GLuint _handle;

	std::map<std::string, GLint> uniformLocations;
	GLint GetUniformLocation(const std::string &name);

	int loadShaderFromFile(std::string filename, SHADER_TYPE types);
};