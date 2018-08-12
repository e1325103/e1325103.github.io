#include "Texture2D.h"

Texture2D::Texture2D(TextureParams &params, const std::string &path) {
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

		_dataType = GL_UNSIGNED_BYTE;

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

void Texture2D::upload() {
	glGenTextures(1, &_handle);
	glBindTexture(GL_TEXTURE_2D, _handle);

	configurate();

	glTexImage2D(GL_TEXTURE_2D, 0, _format, _width, _height, 0, _format, _dataType, _data);

	generateMipMap();
}

void Texture2D::configurate() {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLuint)_params.wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLuint)_params.wrapMode);

	switch (_params.samplingMode)
	{
	case SAMPLING_MODE::NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	default:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	}

	switch (_params.samplingMode)
	{
	case SAMPLING_MODE::NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		break;
	case SAMPLING_MODE::LINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		break;
	case SAMPLING_MODE::BILINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		break;
	case SAMPLING_MODE::TRILINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		break;
	default:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		break;
	}
}

void Texture2D::generateMipMap() {
	// TODO bind(0) if not private
	glGenerateMipmap(GL_TEXTURE_2D);
}