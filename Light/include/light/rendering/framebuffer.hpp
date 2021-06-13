#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include <memory>

namespace Light
{
	struct FramebufferSpec
	{
		uint32_t width, height;
		uint32_t samples = 1;

		bool swapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		Framebuffer() = default;
		virtual ~Framebuffer() = default;

		virtual const FramebufferSpec& getSpec() const = 0;

		virtual uint32_t getColorAttachmentRendererId() const = 0;

		virtual void resize(uint32_t width, uint32_t height) = 0;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		static std::shared_ptr<Framebuffer> create(const FramebufferSpec& spec);
	};	
	
}

#endif // __FRAMEBUFFER_H__