#ifndef __OPENGLTEXTURE_H__
#define __OPENGLTEXTURE_H__

#include "rendering/texture.hpp"

namespace Light
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		~OpenGLTexture2D();

		uint32_t getWidth() const override { return width; }
		uint32_t getHeight() const override { return height; }

		void bind(uint32_t slot = 0) const override;

	private:
		uint32_t rendererId;

		uint32_t width, height;
		std::string path;
	};

	class OpenGLCubemap : public Cubemap
	{
	public:
		OpenGLCubemap(const std::string& path);
		~OpenGLCubemap();

		void bind(uint32_t slot = 0) const override;
	private:
		uint32_t rendererId;
		std::string path;
	};

}

#endif // __OPENGLTEXTURE_H__