#include "light/platform/opengl/opengltexture.hpp"

#include "core/logging.hpp"

#include "glad/gl.h"
#include "stb_image.h"

namespace Light
{
	Texture2D* Texture2D::create(const std::string& path)
	{
		return new OpenGLTexture2D(path);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_path(path)
	{
		int width, height, channels;

		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if(data == NULL)
		{
			LIGHT_CORE_ERROR("Failed to create texture: {}", path);
		}

		this->m_width = width;
		this->m_height = height;

		GLenum internalformat;
		GLenum type;

		if(channels == 3)
		{
			internalformat = GL_RGB8;
			type = GL_RGB;
		}
		else
		{
			internalformat = GL_RGBA8;
			type = GL_RGBA;
		}

		glGenTextures(1, &m_rendererId);
		glBindTexture(GL_TEXTURE_2D, m_rendererId);
		glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, type, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		stbi_image_free(data);

	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_rendererId);
	}

	void OpenGLTexture2D::bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_rendererId);
	}

	Cubemap* Cubemap::create(const std::string& path)
	{
		return new OpenGLCubemap(path);
	}

	OpenGLCubemap::OpenGLCubemap(const std::string& path) : m_path(path)
	{
		std::vector<std::string> facePaths = {
			"/right.jpg",
			"/left.jpg",
			"/top.jpg",
			"/bottom.jpg",
			"/front.jpg",
			"/back.jpg"
		};

		glGenTextures(1, &m_rendererId);
    	glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererId);

		stbi_set_flip_vertically_on_load(false);

		int width, height, channels;
		for(unsigned int i = 0; i < facePaths.size(); i++)
		{
			stbi_uc* data = stbi_load((path + facePaths[i]).c_str(), &width, &height, &channels, 0);

			if(data == NULL)
			{
				LIGHT_CORE_ERROR("Failed to create cubemap texture: {}", path + facePaths[i]);
			}

			GLenum internalformat;
			GLenum type;

			if(channels == 3)
			{
				internalformat = GL_RGB8;
				type = GL_RGB;
			}
			else
			{
				internalformat = GL_RGBA8;
				type = GL_RGBA;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalformat, width, height, 0, type, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	OpenGLCubemap::~OpenGLCubemap()
	{
		glDeleteTextures(1, &m_rendererId);
	}

	void OpenGLCubemap::bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererId);
	}
}
