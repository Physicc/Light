#ifndef __OPENGLFRAMEBUFFER_H__
#define __OPENGLFRAMEBUFFER_H__

#include "core/base.hpp"
#include "light/rendering/framebuffer.hpp"

namespace Light
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpec& spec);
		~OpenGLFramebuffer();

		inline const FramebufferSpec& getSpec() const override { return m_spec; }

		void bind() override;
		void unbind() override;

		void invalidate();
		void resize(uint32_t width, uint32_t height) override;

		int readPixelInt(uint32_t attachmentIndex, uint32_t x, uint32_t y) override;
		glm::vec4 readPixelVec4(uint32_t attachmentIndex, uint32_t x, uint32_t y) override;

		void clearAttachment(uint32_t attachmentIndex, int clearValue) override;
		void clearAttachment(uint32_t attachmentIndex, glm::vec4 clearValue) override;
		void clearDepthAttachment() override;

		inline uint32_t getColorAttachmentRendererId(uint32_t attachmentIndex = 0) const override
		{
			return m_colorAttachmentIds[attachmentIndex];
		}

		inline uint32_t getRendererId() const override { return m_rendererId; }

		virtual void bindAttachmentTexture(uint32_t attachmentIndex, uint32_t slot) override;
		virtual void bindDepthAttachmentTexture(uint32_t slot) override;

	private:
		FramebufferSpec m_spec;
		std::vector<FramebufferTextureSpec> m_colorAttachmentSpecs;
		FramebufferTextureSpec m_depthAttachmentSpec;

		uint32_t m_rendererId = 0;

		std::vector<uint32_t> m_colorAttachmentIds;
		uint32_t m_depthAttachmentId = 0;
	};

}

#endif // __OPENGLFRAMEBUFFER_H__
