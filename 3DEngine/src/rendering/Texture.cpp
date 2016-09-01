#include "Texture.h"

#include <regex>
#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "../core/ResourcesMgr.h"
#include "../core/Debug.h"

Texture::Texture(GLenum target, GLint minFilter, GLint magFileter, GLint wrap) : m_path("UNKNOWN"), m_target(target)
{
	glGenTextures(1, &m_id);

	glBindTexture(m_target, m_id);
	setWrap(wrap);
	setMinFilter(minFilter);
	setMagFilter(magFileter);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_id);
	Debug::info("Released texture '%s'", m_path.c_str());
}

void Texture::bind(int samplerLocation)
{
	glActiveTexture(GL_TEXTURE0 + samplerLocation);
	glBindTexture(m_target, m_id);
}

void Texture::setImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)
{
	glBindTexture(m_target, m_id);
	glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);

	if (m_minFilter == GL_LINEAR_MIPMAP_LINEAR  || m_minFilter == GL_NEAREST_MIPMAP_NEAREST ||
		m_minFilter == GL_NEAREST_MIPMAP_LINEAR || m_minFilter == GL_LINEAR_MIPMAP_NEAREST)
	{
		glGenerateMipmap(target);

	}
	GLfloat maxAnisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	glTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
}

void Texture::setWrap(GLint wrap)
{
	m_wrap = wrap;
	glBindTexture(m_target, m_id);
	setParameteri(GL_TEXTURE_WRAP_S, wrap);
	setParameteri(GL_TEXTURE_WRAP_T, wrap);
	if (m_target == GL_TEXTURE_CUBE_MAP)
	{
		setParameteri(GL_TEXTURE_WRAP_R, wrap);
	}
}

void Texture::setMinFilter(GLint minFilter)
{
	m_minFilter = minFilter;
	glBindTexture(m_target, m_id);
	setParameteri(GL_TEXTURE_MIN_FILTER, minFilter);
}

void Texture::setMagFilter(GLint magFilter)
{
	m_magFilter = magFilter;
	glBindTexture(m_target, m_id);
	setParameteri(GL_TEXTURE_MAG_FILTER, magFilter);
}

void Texture::setParameteri(GLenum pname, GLint param)
{
	glTexParameteri(m_target, pname, param);
}

bool Texture::loadFromFile(const std::string& path)
{
	m_path = path;
	try
	{
		glBindTexture(m_target, m_id);

		int width, height;
		unsigned char* image;

		if (m_target == GL_TEXTURE_2D)
		{
			image = loadRawImage(path, width, height);
			setImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			stbi_image_free(image);
		}
		else if (m_target == GL_TEXTURE_CUBE_MAP)
		{
			std::string faces[6] = { "right", "left", "top", "bottom", "back", "front" };
			for (GLuint i = 0; i < 6; ++i)
			{
				image = loadRawImage(path + "/" + faces[i] + ".png", width, height);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
				stbi_image_free(image);
			}
		}
		else
			throw std::runtime_error("Unsupported texture target");

	}
	catch (const std::exception& e)
	{
		Debug::error("Failed to load texture '" + path + "'. " + e.what());
		return false;
	}

	Debug::info("Loaded texture '%s'", path.c_str());

	return true;
}

unsigned char* Texture::loadRawImage(const std::string& path, int& width, int& height)
{
	std::string fullPath = ResourcesMgr::PATH_PREFIX + path;

	int bytesPerPixel;
	unsigned char* image = stbi_load(fullPath.c_str(), &width, &height, &bytesPerPixel, 4);

	if (image == nullptr)
	{
		std::smatch m;
		if (!std::regex_match(fullPath, m, std::regex("(.+\\.)[a-zA-Z]+")))
			throw std::runtime_error("Invalid path.");
		std::string basePath = m[1];

		const size_t extCnt = 4;
		std::string otherExt[extCnt] = { "tga", "png", "jpg", "tif" };
		for (size_t i = 0; i < extCnt && image == nullptr; ++i)
		{
			std::string& ext = otherExt[i];
			image = stbi_load((basePath + ext).c_str(), &width, &height, &bytesPerPixel, 4);
			if (image != nullptr)
				Debug::warning("Couldn't load image '" + path + "' but loaded with ." + ext + " extension");
		}
	}

	if (image == nullptr)
		throw std::runtime_error("Couldn't load image file '" + path + "'");

	return image;
}
