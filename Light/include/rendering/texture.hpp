#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "core/base.hpp"

namespace Light
{
	class Texture
	{
	public:
		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		virtual void bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		Texture2D() {}
		virtual ~Texture2D() = default;
		static Texture2D* create(const std::string& path);

		virtual uint32_t getRendererId() const = 0;
	};

	class Cubemap
	{
	public:
		Cubemap() {}
		virtual ~Cubemap() = default;

		virtual void bind(uint32_t slot = 0) const = 0;
		static Cubemap* create(const std::string& path);
	};

}

#endif // __TEXTURE_H__