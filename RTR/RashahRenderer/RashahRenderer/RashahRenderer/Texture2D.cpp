#include "Texture2D.h"

Texture2D::Texture2D(const std::string &path) {
	loadFromFile(path);
}

Texture2D::~Texture2D() {
	stbi_image_free(_data);
}

void Texture2D::loadFromFile(const std::string &path) {

	int width, height, channelCount;
	stbi_set_flip_vertically_on_load(true);
	auto data = stbi_load(path.c_str(), &width, &height, &channelCount, 0);
	if (nullptr != data)
	{
		_data = data;

		//_data_type = GL_UNSIGNED_BYTE;

		_width = width;
		_height = height;
		switch (channelCount)
		{
		case 1:
			_format = GL_RED;
			break;
		case 2:
			_format = GL_RG;
			break;
		case 3:
			_format = GL_RGB;
			break;
		case 4:
			_format = GL_RGBA;
			break;
		default:
			std::cout << "Failed to assign number of color channels:  " << path << std::endl;
			break;
		}
	}
	else {
		std::cout << "Failed to load image: " << path << std::endl;
	}
}

void Texture2D::bind(GLuint unit) {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, _handle);
}

void Texture2D::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}