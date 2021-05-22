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

		uint32_t getRendererId() const override { return m_rendererId; }

		uint32_t getWidth() const override { return m_width; }
		uint32_t getHeight() const override { return m_height; }

		void bind(uint32_t slot = 0) const override;

	private:
		uint32_t m_rendererId;

		uint32_t m_width, m_height;
		std::string m_path;
	};

	class OpenGLCubemap : public Cubemap
	{
	public:
		OpenGLCubemap(const std::string& path);
		~OpenGLCubemap();

		void bind(uint32_t slot = 0) const override;
	private:
		uint32_t m_rendererId;
		std::string m_path;
	};

}

#endif // __OPENGLTEXTURE_H__