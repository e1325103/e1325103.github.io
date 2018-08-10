#include "Shader.h"

const std::string Shader::SHADER_EXTENSION[] = { ".vert", ".frag" };
const GLenum Shader::SHADER_GL_TYPE[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };

Shader::Shader(std::string file, SHADER_TYPE types[]) {
	int vertexHandle = loadShaderFromFile(file, SHADER_TYPE::VERTEX);
	int fragmentHandle = loadShaderFromFile(file, SHADER_TYPE::FRAGMENT);

	_handle = glCreateProgram();

	std::vector<GLuint> shaderHandles;

	if (vertexHandle >= 0) {
		shaderHandles.push_back((GLuint)vertexHandle);
	}
	if (fragmentHandle >= 0) {
		shaderHandles.push_back((GLuint)fragmentHandle);
	}

	for (const auto i : shaderHandles) {
		glAttachShader(_handle, i);
	}

	glLinkProgram(_handle);

	GLint success = 0;
	glGetProgramiv(_handle, GL_LINK_STATUS, &success);

	if (success == GL_FALSE) {
		GLint logSize;
		glGetProgramiv(_handle, GL_INFO_LOG_LENGTH, &logSize);

		char* message = new char[logSize];
		glGetProgramInfoLog(_handle, logSize, nullptr, message);

		std::cout << "Shader Link Error:" << std::endl;
		std::cout << message << std::endl;

		glDeleteProgram(_handle);
		for (const auto i : shaderHandles) {
			glDeleteShader(i);
		}

		delete[] message;
		exit(EXIT_FAILURE);
	}

	for (const auto i : shaderHandles) {
		glDetachShader(_handle, i);
		glDeleteShader(i);
	}
}

Shader::~Shader()
{
	glUseProgram(0);
	glDeleteProgram(_handle);
}

int Shader::loadShaderFromFile(std::string filename, SHADER_TYPE type) {
	std::ifstream file(filename + Shader::SHADER_EXTENSION[(int)type]);

	int handle = -1;

	if (file.good()) {
		std::string code = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
		file.close();

		handle = glCreateShader(Shader::SHADER_GL_TYPE[(int)type]);

		if (handle == 0) {
			std::cout << "Failed to create " << Shader::SHADER_GL_TYPE[(int)type] << std::endl;
			system("PAUSE");
			exit(EXIT_FAILURE);
		}

		const char* codePtr = code.c_str();
		glShaderSource(handle, 1, &codePtr, 0);
		glCompileShader(handle);

		GLint success;
		glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE || !glIsShader(handle)) {
			GLint logSize;
			glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logSize);

			char* message = new char[logSize];
			glGetShaderInfoLog(handle, logSize, nullptr, message);

			std::cout << "Shader Compile Error: " << filename + Shader::SHADER_EXTENSION[(int)type] << std::endl;
			std::cout << message << std::endl;

			glDeleteShader(handle);

			delete[] message;
			exit(EXIT_FAILURE);
		}
	}
	return handle;
}

void Shader::bind() {
	glUseProgram(_handle);
}

void Shader::unbind() {
	glUseProgram(0);
}

void Shader::setUniform(std::string name, int i)
{
	glProgramUniform1i(_handle, GetUniformLocation(name), i);
}

void Shader::setUniform(std::string name, unsigned int i)
{
	glProgramUniform1ui(_handle, GetUniformLocation(name), i);
}

void Shader::setUniform(std::string name, float f)
{
	glProgramUniform1f(_handle, GetUniformLocation(name), f);
}

void Shader::setUniform(std::string name, const glm::mat4& mat)
{
	glProgramUniformMatrix4fv(_handle, GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setUniform(std::string name, const glm::mat3& mat)
{
	glProgramUniformMatrix3fv(_handle, GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setUniform(std::string name, const glm::vec2& vec)
{
	glProgramUniform2f(_handle, GetUniformLocation(name), vec.x, vec.y);
}

void Shader::setUniform(std::string name, const glm::vec3& vec)
{
	glProgramUniform3f(_handle, GetUniformLocation(name), vec.x, vec.y, vec.z);
}

GLint Shader::GetUniformLocation(std::string& name)
{
	const auto location = uniformLocations.find(name);
	if (location == uniformLocations.end())
	{

		glUseProgram(_handle);
		uniformLocations[name] = glGetUniformLocation(_handle, name.c_str());
		return uniformLocations[name];
	}
	return location->second;
}